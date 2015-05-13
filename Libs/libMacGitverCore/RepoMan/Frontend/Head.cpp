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

#include "RepoMan/Events.hpp"

#include "RepoMan/Head.hpp"

#include "RepoMan/Data/HeadData.hpp"

#include "RepoMan/Branch.hpp"
#include "RepoMan/Repo.hpp"

#include "libGitWrap/Repository.hpp"

namespace RM
{

    namespace Frontend
    {

        Head::Head(const Git::Repository& repo, Base* parent)
            : Base(*new Internal::HeadPrivate(this, repo))
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

    }

}
