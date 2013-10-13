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

#ifndef REPOMAN_REF_TREE_NODE_PRIVATE_HPP
#define REPOMAN_REF_TREE_NODE_PRIVATE_HPP

#include "RepoMan/Private/BasePrivate.hpp"

#include "RepoMan/RefTreeNode.hpp"

namespace RM
{

    namespace Internal
    {

        class RefTreeNodePrivate : public BasePrivate
        {
        public:
            RefTreeNodePrivate(RefTreeNode* _pub, const QString& _name);

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            void postCreation();
            void preTerminate();
            QString displayName() const;
            void dumpSelf(Dumper& dumper) const;

        public:
            QString name;
        };

    }

}

#endif