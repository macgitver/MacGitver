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

#include "RepoMan/Data/Base.hpp"

#include "RepoMan/Frontend/Remote.hpp"

namespace RM
{

    namespace Data
    {

        class Head;

        class Remote
                : public Base
        {
        public:
            static const_or_constexpr ObjTypes StaticObjectType = ObjTypes::Remote;

        public:
            Remote(Remote* _pub, const Git::Remote& _obj);

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            void postCreation();
            void preTerminate();
            QString displayName() const;
            void dumpSelf(Internal::Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            QString         name;
            Head*           mHead;                 //!< The HEAD
        };

    }

}
