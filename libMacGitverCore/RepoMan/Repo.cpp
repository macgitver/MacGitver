/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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
#include "libGitWrap/Submodule.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "RepoMan/Events.hpp"
#include "RepoMan/Repo.hpp"
#include "RepoMan/RepoMan.hpp"
#include "RepoMan/Ref.hpp"
#include "RepoMan/Dumper.hpp"

namespace RM
{

    Repo::Repo(const Git::Repository& repo, Base* parent)
        : Base(parent)
    {
        mRepo = repo;
        mPath = repo.basePath();
        mIsLoaded = mRepo.isValid();
        mIsActive = false;
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
            if (mPath.isEmpty()) {
                mDisplayAlias = trUtf8("Unknown Repository");
            }
            else {
                QStringList sl = mPath.split( QChar( L'/' ), QString::SkipEmptyParts );
                mDisplayAlias = sl.last();
            }
        }

        // Do an initial refresh
        refresh();
    }

    Repo::~Repo()
    {
        if (mIsLoaded) {
            unload();
        }
        if (mParent) {
            mParent->removeChild( this );
        }

        MacGitver::repoMan().internalClosedRepo(this);
    }

    Git::Repository Repo::gitRepo()
    {
        if (ensureIsLoaded()) {
            return mRepo;
        }

        return Git::Repository();
    }

    Git::Repository Repo::gitLoadedRepo() const
    {
        return mIsLoaded ? mRepo : Git::Repository();
    }

    QString Repo::path() const
    {
        return mPath;
    }

    bool Repo::isSubModule() const
    {
        return mIsSubModule;
    }

    bool Repo::isBare() const
    {
        return mIsBare;
    }

    bool Repo::isLoaded() const
    {
        return mIsLoaded;
    }

    bool Repo::isActive() const
    {
        return mIsActive;
    }

    Repo* Repo::parentRepository()
    {
        return mParent;
    }

    Repo::List Repo::submodules() const
    {
        return mChildren;
    }

    void Repo::activated()
    {
        Q_ASSERT(!mIsActive);

        if(mUnloadTimer) {
            mUnloadTimer->stop();
            mUnloadTimer->deleteLater();
            mUnloadTimer = NULL;
        }
        mIsActive = true;
    }

    void Repo::deactivated()
    {
        Q_ASSERT(mIsActive);

        Q_ASSERT(!mUnloadTimer);
        mUnloadTimer = new QTimer(this);
        connect( mUnloadTimer, SIGNAL(timeout()), this, SLOT(unloadTimer()) );
        mUnloadTimer->setInterval(15 * 60 * 1000); // quarter of an hour
        mUnloadTimer->start();

        mIsActive = false;
    }

    QString Repo::displayAlias() const
    {
        return mDisplayAlias;
    }

    void Repo::setDisplayAlias( const QString& alias )
    {
        if( mDisplayAlias != alias )
        {
            mDisplayAlias = alias;
            emit aliasChanged( alias );
        }
    }

    void Repo::load()
    {
        Q_ASSERT( !mIsLoaded );
    }

    void Repo::unload()
    {
        if( mIsActive )
        {
            qDebug() << "Unloading active RepoInfo. Will deactivate it first.";
            MacGitver::repoMan().activate(NULL);
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

    bool Repo::ensureIsLoaded()
    {
        if (!mIsLoaded) {
            load();
        }

        return mIsLoaded;
    }

    void Repo::unloadTimer()
    {
        Q_ASSERT(mUnloadTimer);

        mUnloadTimer->stop();
        mUnloadTimer->deleteLater();
        mUnloadTimer = NULL;

        unload();
    }

    void Repo::removeChild( Repo* child )
    {
        for( int i = 0; i < mChildren.count(); i++ )
        {
            if( mChildren.at( i ) == child )
            {
                mChildren.remove( i );
                emit childRemoved( this, child );
                return;
            }
        }
    }

    void Repo::close()
    {
        if (mIsActive) {
            MacGitver::repoMan().activate(NULL);
        }

        Events::self()->repositoryAboutToClose(this);
        emit aboutToClose(this);

        foreach(Repo* child, mChildren) {
            child->close();
        }

        terminateObject();
    }

    Repo* Repo::repoByPath( const QString& basePath, bool searchSubmodules )
    {
        foreach( Repo* info, mChildren )
        {
            if( info->path() == basePath )
            {
                return info;
            }

            if( searchSubmodules )
            {
                if( Repo* sub = info->repoByPath( basePath, true ) )
                {
                    return sub;
                }
            }
        }

        return NULL;
    }

    void Repo::scanSubmodules()
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

        foreach (Git::Submodule sub, subs) {
            Git::Result child;
            Git::Repository subRepo = sub.subRepository(child);
            if (!child) {
                continue;
            }

            Repo* subInfo = NULL;
            QString path = subRepo.basePath();

            subInfo = repoByPath( path, true );
            if( !subInfo )
            {
                subInfo = new Repo(subRepo, this);
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
            else {
                int i = oldChildren.indexOf(subInfo);
                if (i != -1) {
                    oldChildren.remove(i);
                }
            }
        }

        foreach( Repo* info, oldChildren )
        {
            removeChild( info );
        }
    }

    QString Repo::branchDisplay() const
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
                    return trUtf8( "detached at <b>%1</b>" ).arg(HEAD.objectId(r).toString());
                }
            }
            else
            {
                return trUtf8( "<b style=\"color: red;\">Branch yet to be born</b>" );
            }
        }
        return tr( "&lt;unknown&gt;" );
    }

    void Repo::findAlias()
    {
    }

    bool Repo::refreshSelf()
    {
        if (!mIsLoaded) {
            // In case we're not loaded, just return and do nothing.
            return true;
        }

        return true;
    }

    void Repo::postRefreshChildren()
    {
        if (!mIsLoaded) {
            // In case we're not loaded, just return and do nothing.
            return;
        }

        Git::Result r;
        Git::ReferenceList refs = mRepo.allReferences(r);
        foreach (Git::Reference ref, refs) {
            QString pfx = ref.prefix();
            Base* parent = findRefParent(pfx.split(QChar(L'/')), true);

            Ref* rmRef = new Ref(parent, BranchType, ref.shorthand());
        }
    }

    ObjTypes Repo::objType() const
    {
        return RepoObject;
    }

    void Repo::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Repository 0x%1 - %02"))
                       .arg(quintptr(this),0,16)
                       .arg(isLoaded() ? gitLoadedRepo().name() : QLatin1String("<not loaded>")));
    }

}
