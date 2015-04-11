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

#include "libRepoMan/Frontend/Branch.hpp"
#include "libRepoMan/Frontend/Repo.hpp"
#include "libRepoMan/Frontend/Head.hpp"

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libRepoMan/Data/Branch.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Branch::Branch(Base* parent, const Git::Reference &ref)
            : Reference( *new Data::Branch(this, ref) )
        {
            RM_D(Branch);
            d->linkToParent( parent );
        }

        QString Branch::upstreamRefName() const
        {
            RM_CD(Branch);
            return d->mUpstreamRefName;
        }

        Reference Branch::upstream()
        {
            return repository().findReference(upstreamRefName());
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

        bool Branch::isHead() const
        {
            const Repo* r = repository();
            const Head* h = r ? r->head() : NULL;
            return h && h->is(this);
        }
        #endif
    }

#if 0
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
        dumper.addLine(QString(QStringLiteral("Branch 0x%1 - %2"))
                       .arg(quintptr(mPub),0,16)
                       .arg(mName));
    }

    QString BranchPrivate::objectTypeName() const
    {
        return QStringLiteral("Branch");
    }

    void BranchPrivate::emitMoved()
    {
        if (!repoEventsBlocked()) {
            Events::self()->refMoved(mRepo, pub<Ref>());
            Events::self()->branchMoved(mRepo, pub<Branch>());
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

#endif

}
