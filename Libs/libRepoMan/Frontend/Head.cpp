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

#include "libRepoMan/Frontend/Head.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libRepoMan/Data/Head.hpp"

#include "libRepoMan/Frontend/Branch.hpp"
#include "libRepoMan/Frontend/Repo.hpp"

#include "libGitWrap/Repository.hpp"
#include "libGitWrap/BranchRef.hpp"

namespace RM
{

    namespace Data
    {

        #if 0

        HeadPrivate::HeadPrivate(Head* _pub, const Git::Repository& repo)
            : BasePrivate(_pub)
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

        ObjTypes HeadPrivate::objType() const
        {
            return HeadObject;
        }

        void HeadPrivate::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Head 0x%1 - %2"))
                           .arg(quintptr(mPub),0,16)
                           .arg(symbolicName));
        }

        bool HeadPrivate::refreshSelf()
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

        QString HeadPrivate::displayName() const
        {
            return symbolicName;
        }

        QString HeadPrivate::objectTypeName() const
        {
            return QStringLiteral("Head");
        }

        bool HeadPrivate::inherits(ObjTypes type) const
        {
            return type == HeadObject || BasePrivate::inherits(type);
        }
        #endif

    }

    namespace Frontend
    {
        #if 0
        Head::Head(const Git::Repository& ref, Base* parent)
            : Base(*new Internal::HeadPrivate(this, ref))
        {
            RM_D(Head);
            d->linkToParent(parent);
        }

        QString Head::symbolicName() const
        {
            RM_CD(Head);
            if (d->isDetached) {
                return QString();
            }
            return d->symbolicName;
        }

        bool Head::isDetached() const
        {
            RM_CD(Head);
            return d->isDetached;
        }

        bool Head::isUnborn() const
        {
            RM_CD(Head);
            return d->isUnborn;
        }

        Git::ObjectId Head::detachedId() const
        {
            RM_CD(Head);
            return d->detachedId;
        }

        bool Head::is(const Branch* ref) const
        {
            RM_CD(Head);

            if (d->isDetached || d->isUnborn) {
                /* If it's unborn, it cannot match an existing branch anyway */
                return false;
            }

            return ref && d->symbolicName == ref->fullName();
        }

        #endif

    }
}
