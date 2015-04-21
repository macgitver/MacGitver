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

#include "RepoMan/Frontend/Reference.hpp"

namespace RM
{

    namespace Data
    {

        class Reference
                : public Base
        {
        public:
            static const_or_constexpr ObjTypes StaticObjectType = ObjTypes::Reference;

        public:
            Reference(Reference* pub, RefTypes type, const Git::Reference& ref);

        public:
            ObjTypes objType() const;
            QString displayName() const;
            bool refreshSelf();
            void postCreation();
            void preTerminate();
            virtual bool refreshDetails(const Git::Reference& ref);
            virtual void emitMoved();
            void dumpSelf(Internal::Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            RefTypes            mType;
            QString             mFullQualifiedName;
            QString             mName;
            Git::ObjectId       mId;
        };

        GW_DEPRECATED
        typedef Reference Ref;

    }

}
