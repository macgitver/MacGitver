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

#include "RepoMan/Data/Repo.hpp"

#include "RepoMan/Private/Dumper.hpp"

namespace RM
{

    namespace Data
    {

        Repo::Repo(const Git::Repository& repo)
            : mHead(nullptr)
            , mRepo(repo)
        {
        }

        Repo::~Repo()
        {
        }

        Git::Repository Repo::gitRepo(bool doLoad)
        {
            if (!mRepo && doLoad) {
                Q_ASSERT(false); // ###REPOMAN - Not sure if we still want to support this...
            }

            return mRepo;
        }

        /**
         * @brief       Get this object's type
         *
         * @return      always ObjTypes::Repo.
         *
         */
        ObjTypes Repo::objType() const
        {
            return ObjTypes::Repo;
        }

        Repo::SPtr Repo::repoByPath(const QString& basePath, bool searchSubmodules)
        {
            for (const Repo::SPtr& subRepo : mSubmodules) {
                if (subRepo->path() == basePath) {
                    return subRepo;
                }

                if (searchSubmodules) {
                    Repo::SPtr found = subRepo->repoByPath(basePath, true);
                    if (found) {
                        return found;
                    }
                }
            }
            return Repo::SPtr();
        }

        QString Repo::displayName() const
        {
            return mDisplayAlias;
        }

        void Repo::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Repository 0x%1 - %02"))
                           .arg(quintptr(this),0,16)
                           .arg(mIsLoaded ? mRepo.name()
                                          : QStringLiteral("<not loaded>")));
        }

        QString Repo::objectTypeName() const
        {
            return QStringLiteral("Repo");
        }

        bool Repo::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Repo || Base::inherits(type);
        }

    }

}
