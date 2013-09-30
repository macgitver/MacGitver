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
#include "RepoMan/Events.hpp"
#include "RepoMan/Dumper.hpp"

namespace RM
{

    /**
     * @class       RepoMan
     * @brief       Manages the open / active repositories
     */

    /**
     * @brief       Constructor
     *
     * Initializes the repository subsystem. Invoked from MacGitver boot up code.
     *
     */
    RepoMan::RepoMan()
        : Base(NULL)
    {
        Events::addReceiver(this);
        mActiveRepo = NULL;
    }

    /**
     * @brief       Destructor
     *
     * Close all repositories.
     *
     */
    RepoMan::~RepoMan()
    {
        closeAll();
        Events::delReceiver(this);
    }

    /**
     * @brief       Open a repository (By it's path)
     *
     * Opens a Git::Repository at the @a path and if this is valid, call open() to setup a RepoMan
     * repository for it.
     *
     * @param[in]   path    The path where to look for the repository to open.
     *
     * @return      A pointer to a Repo object that represents the repositorty. An existing one will
     *              be returned if such a Repo object can be found.
     *              `NULL` will be returned, if the repository cannot be opened.
     *
     */
    Repo* RepoMan::open(const QString& path)
    {
        Git::Result result;
        Git::Repository repo = Git::Repository::open( path, result );
        if( !result || !repo.isValid() )
        {
            return NULL;
        }

        return open( repo );
    }

    /**
     * @brief       Setup a Repo object for a Git::Repository
     *
     * If there is no Repo object for the git repository at that place in the file system, a new one
     * will be created and registered with the repo-manager and the core.
     *
     * @param[in]   gitRepo The Git::Repository object
     *
     * @return      Returns either an existing Repo object or a newly created one.
     *
     */
    Repo* RepoMan::open(const Git::Repository& gitRepo)
    {
        Repo* repo = repoByPath( gitRepo.basePath(), false );

        if(!repo) {
            repo = new Repo(gitRepo);
            mRepos.append(repo);

            Events::self()->repositoryOpened(repo);

            // we need to scan for submodules explicitly, since we didn't call load() on the Repo*
            repo->scanSubmodules();

            if (mRepos.count() == 1) {
                emit firstRepositoryOpened();
            }
        }

        activate( repo );
        return repo;
    }

    Repo* RepoMan::repoByPath( const QString& basePath, bool searchSubmodules )
    {
        foreach(Repo* repo, mRepos) {
            if (repo->path() == basePath) {
                return repo;
            }

            if (searchSubmodules) {
                if (Repo* sub = repo->repoByPath(basePath, true)) {
                    return sub;
                }
            }
        }

        return NULL;
    }

    /**
     * @brief       Close all currently open repositories
     *
     */
    void RepoMan::closeAll()
    {
        foreach(Repo* repo, mRepos) {
            repo->close();
        }
    }

    void RepoMan::activate(Repo* repository)
    {
        if(repository != mActiveRepo) {
            Repo* old = mActiveRepo;

            if (mActiveRepo) {
                mActiveRepo->deactivated();
                Events::self()->repositoryDeactivated(mActiveRepo);
            }

            mActiveRepo = repository;

            if (repository) {
                repository->activated();
                Events::self()->repositoryActivated(mActiveRepo);
            }

            // Finally emit a signal that just tells about the change
            if ((mActiveRepo != NULL) != (old != NULL)) {
                emit hasActiveRepositoryChanged(mActiveRepo != NULL);
            }
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

    Repo* RepoMan::activeRepository()
    {
        return mActiveRepo;
    }

    Repo::List RepoMan::repositories() const
    {
        return mRepos;
    }

    bool RepoMan::refreshSelf()
    {
        return true;
    }

    ObjTypes RepoMan::objType() const
    {
        return RepoManagerObject;
    }

    void RepoMan::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QLatin1String("Repository-Manager"));
    }

}
