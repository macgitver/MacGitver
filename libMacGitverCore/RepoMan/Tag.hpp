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

#ifndef MGV_CORE_REPOMAN_TAG_HPP
#define MGV_CORE_REPOMAN_TAG_HPP

#include "Ref.hpp"

namespace RM
{

    class MGV_CORE_API Tag : public Ref
    {
    public:
        enum { StaticObjectType = TagObject };

    public:
        Tag(Base* parent, const QString& name);

    public:

    private:
        ObjTypes objType() const;
        void dumpSelf(Internal::Dumper& dumper) const;

    private:
    };

}

#endif