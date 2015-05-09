/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#include "libRepoMan/Backend/RepoMan.hpp"

#include "libRepoMan/Backend/RepoLocker.hpp"
#include "libRepoMan/Services/RefreshRepository.hpp"

#include "libGitWrap/Submodule.hpp"

#include <QDebug>

namespace RM { namespace Services {

RefreshRepository::RefreshRepository(const Frontend::Repo& repo)
    : mRepo()
{

}

RefreshRepository::RefreshRepository(const Data::Repo::SPtr& repo)
    : mRepo(repo)
{
}

void RefreshRepository::run()
{
    Backend::RepoLocker lock(mRepo);

    mGitRepo = mRepo->gitRepo(false);
    if (!mGitRepo.isValid()) {
        return;
    }

    scanSubmodules();
}


void RefreshRepository::scanSubmodules()
{
    Git::Result r;
    Git::Submodule::List subs = mGitRepo.submodules(r);
    if (!r) {
        return;
    }

    Data::Repo::SList oldSubmodules = Data::sharedFromWeakList<Data::Repo>(mRepo->submodules());

    foreach (Git::Submodule sub, subs) {
        Git::Result child;

        Git::Repository subRepo = sub.subRepository(child);
        if (!child) {
            // If we cannot load the repository, keep it in list of submodules to remove.
            continue;
        }
        Q_ASSERT(subRepo.isValid());

        QString path = subRepo.workTreePath();

        if (path.endsWith(L'/')) {
            path = path.left(path.length() - 1);
        }

        auto it = std::find(oldSubmodules.begin(), oldSubmodules.end(),
                            mRepo->repoByPath(path, true));
        if (it != oldSubmodules.end()) {
            oldSubmodules.erase(it);
        }
        else {
            // ###REPOMAN
            qDebug() << "Existing Submodule does not exist in data structure";
            // subInfo = new Submodule(subRepo, p);
        }
    }

    foreach (Data::Repo::WPtr repo, oldSubmodules) {
        // ###REPOMAN
        Q_UNUSED(repo);
        // dataOf<Repo>(repo)->terminateObject();
    }
}

} }
