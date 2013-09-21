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

#include <QFileDialog>

#include "libHeaven/Heaven.hpp"
#include "libHeaven/App/PrimaryWindow.hpp"

#include "libMacGitverCore/Config/Config.h"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "RepoMan/RepoMan.hpp"

namespace RM
{

    /**
     * @class       RepoManager
     * @brief       Manages the open / active repositories
     */

    RepoMan::RepoMan()
    {
        mActiveRepo = NULL;
    }

    RepoMan::~RepoMan()
    {
        closeAll();
    }

    void RepoMan::open()
    {
        QWidget* parent = Heaven::primaryWindow();
        QFileDialog *fd = new QFileDialog( parent );

        #ifdef Q_OS_MAC
        fd->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
        #else
        fd->setFileMode(QFileDialog::Directory);
        #endif

        QString lastUsedDir = Config::self().get( "Repository/lastUsedDir", "#" ).toString();
        if( lastUsedDir != QLatin1String( "#" ) )
        {
            fd->setDirectory( lastUsedDir );
        }

        fd->setWindowTitle( trUtf8("Open a git repository") );

        fd->open( this, SLOT(onRepositoryOpenHelper()) );

        #ifdef Q_OS_MAC
        // workaround for osx sheets without a parent
        if (parent == 0)
            fd->exec();
        #endif
    }

    void RepoMan::onRepositoryOpenHelper()
    {
        QFileDialog *fd = qobject_cast<QFileDialog *>(sender());
        Q_ASSERT(fd != 0);

        if ( fd->selectedFiles().isEmpty() )
        {
            return;
        }

        //! @todo error handling
        Git::Result r;
        QString repoDir = Git::Repository::discover( fd->selectedFiles().first(), false,
                                                     QStringList(), r );
        if( repoDir.isEmpty() )
        {
            return;
        }

        //! @todo error handling
        Git::Repository repo = Git::Repository::open( repoDir, r );
        if( !repo.isValid() )
            return;

        // If we successfully loaded the repository at that directory,
        // we store the repository's work dir as "lastUsedDir".
        // If it is a bare repository, we store the repository's directory.
        Config::self().set( "Repository/lastUsedDir", repo.isBare() ? repoDir : repo.basePath() );

        open( repo );
    }

    bool RepoMan::open( const QString& path )
    {
        Git::Result result;
        Git::Repository repo = Git::Repository::open( path, result );
        if( !result || !repo.isValid() )
        {
            return false;
        }

        return open( repo );
    }

    bool RepoMan::open( const Git::Repository& repo )
    {
        RepositoryInfo* info = repoByPath( repo.basePath(), false );

        if( !info )
        {
            info = new RepositoryInfo( repo );
            mRepos.append( info );
            emit repositoryOpened( info );

            // we need to scan for submodules explicitly, since we didn't call load() on the RepoInfo
            info->scanSubmodules();

            if (mRepos.count() == 1) {
                emit firstRepositoryOpened();
            }
        }

        activate( info );

        return true;
    }

    RepositoryInfo* RepoMan::repoByPath( const QString& basePath, bool searchSubmodules )
    {
        foreach( RepositoryInfo* info, mRepos )
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

    void RepoMan::closeAll()
    {
        foreach( RepositoryInfo* repo, mRepos )
        {
            repo->close();
        }
    }

    void RepoMan::activate( RepositoryInfo* repository )
    {
        if( repository == mActiveRepo )
        {
            return;
        }

        if( mActiveRepo )
        {
            mActiveRepo->setActive( false );
        }

        if( repository )
        {
            repository->setActive( true );
        }
    }

    void RepoMan::internalClosedRepo(RepositoryInfo* repository)
    {
        // This pointer is actually useless. THIS IS THE LAST call issued by the destructor of the
        // RepositoryInfo itself. We should probably NOT give this pointer away.

        // However, we need it to find the closed repository in our list. Calling here should have
        // probably happened before the repository is actually destructing itself.

        int i = mRepos.indexOf(repository);
        if (i != -1) {
            mRepos.removeAt(i);
            emit repositoryClosed();

            if (mRepos.count() == 0) {
                emit lastRepositoryClosed();
            }
        }
    }

    /**
     * @internal
     * @brief       internally activate a RepositoryInfo
     *
     * @param[in]   repository      The repository info to activate.
     *
     * Nothing is done if @a repository is already the active repository.
     *
     * This method is called internally from all paths that need to change the active repository - these
     * are more than just the public activation methods.
     *
     */
    void RepoMan::internalActivate( RepositoryInfo* repository )
    {
        if( repository != mActiveRepo )
        {
            RepositoryInfo* old = mActiveRepo;

            if(mActiveRepo)
                emit repositoryDeactivated(mActiveRepo);

            mActiveRepo = repository;

            if(mActiveRepo)
                emit repositoryActivated(mActiveRepo);

            if ((mActiveRepo != NULL) != (old != NULL)) {
                emit hasActiveRepositoryChanged(mActiveRepo != NULL);
            }
        }
    }

    RepositoryInfo* RepoMan::activeRepository()
    {
        return mActiveRepo;
    }

    RepositoryInfo::List RepoMan::repositories() const
    {
        return mRepos;
    }

}
