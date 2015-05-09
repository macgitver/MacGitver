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

#pragma once

#include "RepoMan/Base.hpp"
#include "RepoMan/CollectionNode.hpp"
#include "RepoMan/Branch.hpp"

namespace RM
{

    namespace Internal
    {
        class HeadPrivate;
    }

    class MGV_CORE_API Head : public Base
    {
    public:
        enum { StaticObjectType = HeadObject };
        typedef Internal::HeadPrivate Private;
        typedef QSet< Head* > Set;
        typedef QVector< Head* > List;

    public:
        Head(const Git::Repository& repo, Base* parent);

    public:
        bool isDetached() const;
        bool isUnborn() const;
        Git::ObjectId detachedId() const;
        QString symbolicName() const;

    public:
        bool is(const Branch* ref) const;
    };

}
