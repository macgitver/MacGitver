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

#include "RepoMan/Data/BaseData.hpp"

#include "RepoMan/Head.hpp"

namespace RM
{

    namespace Internal
    {

        class HeadPrivate : public BasePrivate
        {
        public:
            HeadPrivate(Head* pub, const Git::Repository& ref);

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            QString displayName() const;
            void dumpSelf(Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            QString         symbolicName;
            Git::ObjectId   detachedId;
            bool            isDetached      : 1;
            bool            isUnborn        : 1;
        };

    }

}
