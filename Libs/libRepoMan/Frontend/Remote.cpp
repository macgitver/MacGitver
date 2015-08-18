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

#include "libGitWrap/Result.hpp"

#include "libRepoMan/Frontend/Repo.hpp"
#include "libRepoMan/Frontend/Remote.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libRepoMan/Data/Remote.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Remote::Remote(const Git::Remote& gitObj, Base* _parent)
            : Base(*new Data::Remote(this, gitObj))
        {
            RM_D(Remote);

            d->linkToParent(_parent);
        }

        Git::Remote Remote::gitObject()
        {
            Git::Result r;
            return repository()->gitRepo().remote(r, name());
        }

        QString Remote::name() const
        {
            RM_D(Remote);

            return d->name;
        }
        #endif

    }

}

