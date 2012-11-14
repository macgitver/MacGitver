/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include "RepositoryInfo.hpp"

RepositoryInfo::RepositoryInfo()
{
}

RepositoryInfo::~RepositoryInfo()
{
    if( mIsLoaded )
    {
        unload();
    }

    qDeleteAll( mChildren );

    if( mParent )
    {
        mParent->removeChild( this );
    }
}

Git::Repository RepositoryInfo::gitRepo()
{
    if( ensureIsLoaded() )
    {
        return mRepo;
    }

    return Git::Repository();
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

    Q_ASSERT( mUnloadTimer == NULL );
    Q_ASSERT( !mIsActive );

    mIsLoaded = false;
    mRepo = Git::Repository();
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
