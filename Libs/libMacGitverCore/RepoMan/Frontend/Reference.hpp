/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
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

#pragma once

#include "libGitWrap/ObjectId.hpp"

#include "Base.hpp"

namespace RM
{

    namespace Data
    {
        class Reference;
    }

    enum RefTypes
    {
        BranchType,
        TagType
    };

    namespace Frontend
    {

        class MGV_CORE_API Reference
                : public Base
        {
        public:
            static const ObjTypes StaticObjectType = ObjTypes::Reference;
            using DPtrType  = Data::Reference;
            using List      = QList<Reference>;

        protected:
            Reference(Data::Reference* data);

        public:
            Reference();

        public:
            GW_DEPRECATED
            Reference(Base* parent, RefTypes type, const Git::Reference& ref);

            GW_DEPRECATED
            Git::Reference load(Git::Result& r);

        public:
            RefTypes type() const;
            QString name() const;
            QString fullName() const;
            Git::ObjectId id() const;
            QString displaySha1() const;
        };

        inline Reference::Reference()
            : Base()
        {
        }

        GW_DEPRECATED
        typedef Reference Ref;

    }

}
