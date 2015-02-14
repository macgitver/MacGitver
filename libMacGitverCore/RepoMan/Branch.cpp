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

#include "Branch.hpp"
#include "Repo.hpp"
#include "Events.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/BranchPrivate.hpp"

namespace RM
{

    using namespace Internal;

    Branch::Branch(Base* parent, const Git::Reference &ref)
        : Ref( *new BranchPrivate(this, ref) )
    {
        RM_D(Branch);
        d->linkToParent( parent );
    }

    QString Branch::upstreamRefName() const
    {
        RM_CD(Branch);
        return d->mUpstreamRefName;
    }

    Ref* Branch::upstream()
    {
        return repository()->findReference(upstreamRefName());
    }

    bool Branch::hasUpstream() const
    {
        RM_CD(Branch);
        return d->mHasUpstream;
    }

    int Branch::aheadCount() const
    {
        RM_CD(Branch);
        return d->mAheadCount;
    }

    int Branch::behindCount() const
    {
        RM_CD(Branch);
        return d->mBehindCount;
    }

    //-- BranchPrivate -----------------------------------------------------------------------------

    BranchPrivate::BranchPrivate(Branch* pub, const Git::Reference& ref)
        : RefPrivate(pub, BranchType, ref)
        , mHasUpstream(false)
        , mAheadCount(0)
        , mBehindCount(0)
    {
    }

    ObjTypes BranchPrivate::objType() const
    {
        return BranchObject;
    }

    void BranchPrivate::postCreation()
    {
        if (!repoEventsBlocked()) {
            Events::self()->branchCreated(repository(), pub<Branch>());
        }

        RefPrivate::postCreation();
    }

    void BranchPrivate::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->branchAboutToBeDeleted(repository(), pub<Branch>());
        }

        RefPrivate::preTerminate();
    }

    void BranchPrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Branch 0x%1 - %2"))
                       .arg(quintptr(mPub),0,16)
                       .arg(mName));
    }

    QString BranchPrivate::objectTypeName() const
    {
        return QLatin1String("Branch");
    }

    void BranchPrivate::emitMoved(Repo* repo)
    {
        if (!repoEventsBlocked()) {
            Events::self()->refMoved(repo, pub<Ref>());
            Events::self()->branchMoved(repo, pub<Branch>());
        }
    }

    bool BranchPrivate::refreshDetails(const Git::Reference& ref)
    {

        return RefPrivate::refreshDetails(ref);
    }

    bool BranchPrivate::inherits(ObjTypes type) const
    {
        return type == BranchObject || RefPrivate::inherits(type);
    }

}
