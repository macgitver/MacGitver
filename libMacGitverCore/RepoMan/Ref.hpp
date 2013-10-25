/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
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

#ifndef MGV_CORE_REPOMAN_REF_HPP
#define MGV_CORE_REPOMAN_REF_HPP

#include "libGitWrap/ObjectId.hpp"

#include "Base.hpp"

namespace RM
{

    namespace Internal
    {
        class RefPrivate;
    }

    enum RefTypes
    {
        UnknownRefType,

        BranchType,
        TagType,
        RootStageType,

        // Very special refs:
        HEADRefType,
        MERGE_HEADRefType
    };

    class MGV_CORE_API Ref : public Base
    {
    public:
        enum { StaticObjectType = RefObject };

        typedef QSet< Ref* > Set;
        typedef QList< Ref* > List;

    protected:
        Ref(Internal::RefPrivate& _data);

    public:
        Ref(Base* _parent, RefTypes _type, const Git::Reference& _ref);
        ~Ref();

    public:
        RefTypes type() const;
        QString name() const;
        QString fullName() const;
        Git::ObjectId id() const;

    public:
        QString displaySha1() const;
    };

}

#endif
