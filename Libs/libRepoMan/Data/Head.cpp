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

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Data/Head.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libGitWrap/Repository.hpp"
#include "libGitWrap/BranchRef.hpp"

namespace RM
{

    namespace Data
    {

        #if 0
        Head::Head(Frontend::Head* _pub, const Git::Repository& repo)
            : Base(_pub)
            , symbolicName()
            , isDetached(repo.isHeadDetached())
            , isUnborn(repo.isHeadUnborn())
        {
            Git::Result r;

            if (isDetached) {
                detachedId = repo.HEAD(r).objectId();
            }
            else {
                symbolicName = repo.headBranchName(r);
            }
        }

        ObjTypes Head::objType() const
        {
            return ObjTypes::Head;
        }

        void Head::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Head 0x%1 - %2"))
                           .arg(quintptr(mPub),0,16)
                           .arg(symbolicName));
        }

        bool Head::refreshSelf()
        {
            Git::Repository repo = repository()->gitLoadedRepo();
            Git::Result r;

            isUnborn = repo.isHeadUnborn();
            isDetached = repo.isHeadDetached();

            if (isDetached) {
                detachedId = repo.HEAD(r).objectId();
            }
            else {
                symbolicName = repo.headBranchName(r);
            }
            return true;
        }
        #endif

        QString Head::displayName() const
        {
            return mSymbolicName;
        }

        QString Head::objectTypeName() const
        {
            return QStringLiteral("Head");
        }

        bool Head::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Head || Base::inherits(type);
        }

    }

}
