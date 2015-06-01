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

#include "libRepoMan/Data/Branch.hpp"

#include "libRepoMan/Private/Dumper.hpp"

namespace RM
{

    namespace Data
    {

        Branch::Branch(Frontend::Branch* pub, const Git::Reference& ref)
            : Reference(pub, BranchType, ref)
            , mHasUpstream(false)
            , mAheadCount(0)
            , mBehindCount(0)
        {
        }

        ObjTypes Branch::objType() const
        {
            return ObjTypes::Branch;
        }

        #if 0 // ###DEAD
        void Branch::postCreation()
        {
            jif (!repoEventsBlocked()) {
                Events::self()->branchCreated(repository(), pub<Branch>());
            }

            RefPrivate::postCreation();
        }

        void Branch::preTerminate()
        {
            if (!repoEventsBlocked()) {
                Events::self()->branchAboutToBeDeleted(repository(), pub<Branch>());
            }

            RefPrivate::preTerminate();
        }

        void Branch::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Branch 0x%1 - %2"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        QString Branch::objectTypeName() const
        {
            return QStringLiteral("Branch");
        }

        void Branch::emitMoved()
        {
            if (!repoEventsBlocked()) {
                Events::self()->refMoved(mRepo, pub<Ref>());
                Events::self()->branchMoved(mRepo, pub<Branch>());
            }
        }
        #endif

        bool Branch::refreshDetails(const Git::Reference& ref)
        {
            return Reference::refreshDetails(ref);
        }

        bool Branch::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Branch || Reference::inherits(type);
        }

    }

}
