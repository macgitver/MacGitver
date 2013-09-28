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

    Repo* RepoMan::open( const QString& path )
    {
        Git::Result result;
        Git::Repository repo = Git::Repository::open( path, result );
        if( !result || !repo.isValid() )
        {
            return NULL;
        }

        return open( repo );
    }

    Repo* RepoMan::open( const Git::Repository& repo )
    {
        Repo* info = repoByPath( repo.basePath(), false );

        if( !info )
        {
            info = new Repo( repo );
            mRepos.append( info );
            emit repositoryOpened( info );

            // we need to scan for submodules explicitly, since we didn't call load() on the RepoInfo
            info->scanSubmodules();

            if (mRepos.count() == 1) {
                emit firstRepositoryOpened();
            }
        }

        activate( info );

        return info;
    }

    Repo* RepoMan::repoByPath( const QString& basePath, bool searchSubmodules )
    {
        foreach( Repo* info, mRepos )
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

    void RepoMan::closeAll()
    {
        foreach( Repo* repo, mRepos )
        {
            repo->close();
        }
    }

    void RepoMan::activate( Repo* repository )
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

    void RepoMan::internalClosedRepo(Repo* repository)
    {
        // This pointer is actually useless. THIS IS THE LAST call issued by the destructor of the
        // RepositoryInfo itself. We should probably NOT give this pointer away.

        // However, we need it to find the closed repository in our list. Calling here should have
        // probably happened before the repository is actually destructing itself.

        int i = mRepos.indexOf(repository);
        if (i != -1) {
            mRepos.remove(i);
            emit repositoryClosed();

            if (mRepos.count() == 0) {
                emit lastRepositoryClosed();
            }
        }
    }

    /**
     * @internal
     * @brief       internally activate a Repo
     *
     * @param[in]   repository      The repository info to activate.
     *
     * Nothing is done if @a repository is already the active repository.
     *
     * This method is called internally from all paths that need to change the active repository - these
     * are more than just the public activation methods.
     *
     */
    void RepoMan::internalActivate( Repo* repository )
    {
        if( repository != mActiveRepo )
        {
            Repo* old = mActiveRepo;

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

    Repo* RepoMan::activeRepository()
    {
        return mActiveRepo;
    }

    Repo::List RepoMan::repositories() const
    {
        return mRepos;
    }

}
