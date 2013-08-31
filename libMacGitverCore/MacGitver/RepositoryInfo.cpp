/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QDebug>
#include <QTimer>

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectId.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "RepositoryInfo.hpp"
#include "RepoManager.hpp"

RepositoryInfo::RepositoryInfo()
{
}

RepositoryInfo::RepositoryInfo( const Git::Repository& repo )
{
    mRepo = repo;
    mPath = repo.basePath();
    mIsLoaded = mRepo.isValid();
    mIsActive = false;
    mIsDisabled = false;
    mIsBare = mRepo.isValid() && mRepo.isBare();
    mIsSubModule = false;
    mDisplayAlias = QString();
    mParent = NULL;
    mUnloadTimer = NULL;

    if( mPath.endsWith( L'/' ) )
    {
        mPath = mPath.left( mPath.length() - 1 );
    }

    findAlias();

    if( mDisplayAlias.isEmpty() )
    {
        QStringList sl = mPath.split( QChar( L'/' ), QString::SkipEmptyParts );
        mDisplayAlias = sl.last();
    }
}

RepositoryInfo::~RepositoryInfo()
{
    qDeleteAll( mChildren );

    if( mIsLoaded )
    {
        unload();
    }

    if( mParent )
    {
        mParent->removeChild( this );
    }

    MacGitver::repoMan().internalClosedRepo(this);
}

Git::Repository RepositoryInfo::gitRepo()
{
    if( ensureIsLoaded() )
    {
        return mRepo;
    }

    return Git::Repository();
}

QString RepositoryInfo::path() const
{
    return mPath;
}

bool RepositoryInfo::isSubModule() const
{
    return mIsSubModule;
}

bool RepositoryInfo::isBare() const
{
    return mIsBare;
}

bool RepositoryInfo::isLoaded() const
{
    return mIsLoaded;
}

bool RepositoryInfo::isDisabled() const
{
    return mIsDisabled;
}

bool RepositoryInfo::isActive() const
{
    return mIsActive;
}

RepositoryInfo* RepositoryInfo::parentRepository()
{
    return mParent;
}

RepositoryInfo::List RepositoryInfo::children() const
{
    return mChildren;
}

void RepositoryInfo::setActive( bool active )
{
    if( active == mIsActive )
    {
        return;
    }

    if( mIsActive )
    {
        Q_ASSERT( !mUnloadTimer );
        mUnloadTimer = new QTimer( this );
        connect( mUnloadTimer, SIGNAL(timeout()), this, SLOT(unloadTimer()) );
        mUnloadTimer->setInterval( 15 * 60 * 1000 ); // quarter of an hour
        mUnloadTimer->start();

        mIsActive = false;

        MacGitver::repoMan().internalActivate( NULL );
    }
    else
    {
        if( mUnloadTimer )
        {
            mUnloadTimer->stop();
            mUnloadTimer->deleteLater();
            mUnloadTimer = NULL;
        }
        mIsActive = true;

        MacGitver::repoMan().internalActivate( this );
    }
}

QString RepositoryInfo::displayAlias() const
{
    return mDisplayAlias;
}

void RepositoryInfo::setDisplayAlias( const QString& alias )
{
    if( mDisplayAlias != alias )
    {
        mDisplayAlias = alias;
        emit aliasChanged( alias );
    }
}

void RepositoryInfo::load()
{
    Q_ASSERT( !mIsLoaded );
}

void RepositoryInfo::unload()
{
    if( mIsActive )
    {
        qDebug() << "Unloading active RepositoryInfo. Will deactivate it first.";
        setActive( false );
    }
    Q_ASSERT( !mIsActive );

    if( mUnloadTimer )
    {
        mUnloadTimer->stop();
        mUnloadTimer->deleteLater();
        mUnloadTimer = NULL;
    }

    emit aboutToUnload( this );

    mIsLoaded = false;
    mRepo = Git::Repository();

    emit unloaded( this );
}

bool RepositoryInfo::ensureIsLoaded()
{
    if( mIsLoaded )
    {
        return true;
    }

    if( mIsDisabled )
    {
        return false;
    }

    load();
    return mIsLoaded;
}

void RepositoryInfo::unloadTimer()
{
    Q_ASSERT( mUnloadTimer );

    mUnloadTimer->stop();
    mUnloadTimer->deleteLater();
    mUnloadTimer = NULL;

    unload();
}

void RepositoryInfo::removeChild( RepositoryInfo* child )
{
    for( int i = 0; i < mChildren.count(); i++ )
    {
        if( mChildren.at( i ) == child )
        {
            mChildren.removeAt( i );
            emit childRemoved( this, child );
            return;
        }
    }
}

void RepositoryInfo::close()
{
    emit aboutToClose( this );

    foreach( RepositoryInfo* child, mChildren )
    {
        child->close();
    }

    delete this;
}

RepositoryInfo* RepositoryInfo::repoByPath( const QString& basePath, bool searchSubmodules )
{
    foreach( RepositoryInfo* info, mChildren )
    {
        if( info->path() == basePath )
        {
            return info;
        }

        if( searchSubmodules )
        {
            if( RepositoryInfo* sub = info->repoByPath( basePath, true ) )
            {
                return sub;
            }
        }
    }

    return NULL;
}

void RepositoryInfo::scanSubmodules()
{
    if( !ensureIsLoaded() )
    {
        return;
    }

    Git::Result r;
    Git::Submodule::List subs = mRepo.submodules( r );
    if( !r )
    {
        return;
    }

    List oldChildren = mChildren;

    foreach( Git::Submodule sub, subs )
    {
        if( !sub.isOpened() )
        {
            Git::Result r;
            if( !sub.open( r ) || !r )
            {
                continue;
            }
        }
        Q_ASSERT( sub.isOpened() );

        Git::Repository subRepo = sub.repository();
        RepositoryInfo* subInfo = NULL;
        QString path = subRepo.basePath();

        subInfo = repoByPath( path, true );
        if( !subInfo )
        {
            subInfo = new RepositoryInfo( subRepo );
            subInfo->mIsSubModule = true;
            subInfo->setDisplayAlias( subRepo.name() );
            subInfo->mParent = this;
            mChildren.append( subInfo );

            emit childAdded( this, subInfo );

            if( !subInfo->isBare() )
            {
                subInfo->scanSubmodules();
            }
        }
        else
        {
            oldChildren.removeOne( subInfo );
        }
    }

    foreach( RepositoryInfo* info, oldChildren )
    {
        removeChild( info );
    }
}

QString RepositoryInfo::branchDisplay() const
{
    if( mIsLoaded )
    {
        Git::Result r;
        Git::Reference HEAD = mRepo.HEAD( r );

        if( HEAD.isValid() )
        {
            if( HEAD.name() != QLatin1String( "HEAD" ) )
            {
                return trUtf8( "<b style=\"background-color: #FFB54F;"
                               "\">%1</b>" ).arg( HEAD.name().mid( 11 ) );
            }
            else
            {
                return trUtf8( "detached at <b>%1</b>" ).arg( HEAD.objectId( r ).toString() );
            }
        }
        else
        {
            return trUtf8( "<b style=\"color: red;\">Branch yet to be born</b>" );
        }
    }
    return tr( "&lt;unknown&gt;" );
}

void RepositoryInfo::findAlias()
{
}
