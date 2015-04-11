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

#include "libRepoMan/Data/Base.hpp"

#include "libRepoMan/Frontend/Head.hpp"

namespace RM
{

    namespace Data
    {

        class Head
                : public Base
        {
        public:
            static const_or_constexpr ObjTypes StaticObjectType = ObjTypes::Head;

        public:
            Head(Head* pub, const Git::Repository& ref);

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            QString displayName() const;
            void dumpSelf(Internal::Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            bool isDetached() const;
            bool isUnborn() const;
            Git::ObjectId detachedId() const;
            QString symbolicName() const;

        private:
            QString         mSymbolicName;
            Git::ObjectId   mDetachedId;
            bool            mIsDetached      : 1;
            bool            mIsUnborn        : 1;
        };

        inline bool Head::isDetached() const
        {
            return mIsDetached;
        }

        inline bool Head::isUnborn() const
        {
            return mIsUnborn;
        }

        inline Git::ObjectId Head::detachedId() const
        {
            return mDetachedId;
        }

        inline QString Head::symbolicName() const
        {
            return mSymbolicName;
        }

    }

}
