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

    Branch::Branch(Base* _parent, const Git::Reference &_ref)
        : Ref(*new BranchPrivate(this, _ref))
    {
        RM_D(Branch);
        d->linkToParent(_parent);
    }

    QString Branch::upstreamRefName() const
    {
        RM_CD(Branch);
        return d->upstreamRefName;
    }

    Ref* Branch::upstream() const
    {
        return repository()->findReference(upstreamRefName());
    }

    bool Branch::hasUpstream() const
    {
        RM_CD(Branch);
        return d->hasUpstream;
    }

    int Branch::aheadCount() const
    {
        RM_CD(Branch);
        return d->aheadCount;
    }

    int Branch::behindCount() const
    {
        RM_CD(Branch);
        return d->behindCount;
    }

    //-- BranchPrivate -----------------------------------------------------------------------------

    BranchPrivate::BranchPrivate(Branch* _pub, const Git::Reference& _ref)
        : RefPrivate(_pub, BranchType, _ref)
        , hasUpstream(false)
        , aheadCount(0)
        , behindCount(0)
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
                       .arg(name));
    }

    bool BranchPrivate::refreshSelf()
    {
        // ### Update "upstream" and "divergence"
        return RefPrivate::refreshSelf();
    }

    QString BranchPrivate::objectTypeName() const
    {
        return QLatin1String("Branch");
    }

    bool BranchPrivate::inherits(ObjTypes type) const
    {
        return type == BranchObject || RefPrivate::inherits(type);
    }

}
