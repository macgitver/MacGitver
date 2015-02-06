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

#include "RepoMan/Private/BasePrivate.hpp"

#include "RepoMan/Ref.hpp"

namespace RM
{

    namespace Internal
    {

        class RefPrivate : public BasePrivate
        {
        public:
            RefPrivate(Ref* pub, RefTypes type, const Git::Reference& ref);

        public:
            ObjTypes objType() const;
            QString displayName() const;
            virtual bool refreshSelf();
            virtual void postCreation();
            virtual void preTerminate();
            void dumpSelf(Dumper& dumper) const;
            QString objectTypeName() const;
            virtual bool inherits(ObjTypes type) const;

        public:
            RefTypes            mType;
            QString             mFullQualifiedName;
            QString             mName;
            Git::ObjectId       mId;
            QString             mSymbolicTarget;
        };

    }

}

