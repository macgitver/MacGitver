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

#include "libRepoMan/Frontend/Namespace.hpp"

#include "libRepoMan/Private/Dumper.hpp"
#include "libRepoMan/Data/Namespace.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Namespace::Namespace(Base* _parent, const QString& _name)
            : Base(*new Data::Namespace(this, _name))
        {
            RM_D(Namespace);
            d->linkToParent(_parent);
        }

        QString Namespace::name() const
        {
            RM_D(Namespace);

            return d->name;
        }
        #endif

    }

}
