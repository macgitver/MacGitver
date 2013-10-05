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

#ifndef MGV_CORE_REPOMAN_COLLECTION_NODE_HPP
#define MGV_CORE_REPOMAN_COLLECTION_NODE_HPP

#include <QCoreApplication>

#include "Base.hpp"

namespace RM
{

    enum CollectionTypes
    {
        ctBranches,
        ctTags,
        ctNamespaces,
        ctNotes
    };

    class MGV_CORE_API CollectionNode : public Base
    {
        Q_DECLARE_TR_FUNCTIONS(RM_CollectionNode)

    public:
        enum { StaticObjectType = CollectionNodeObject };
        typedef QSet< CollectionNode* > Set;
        typedef QVector< CollectionNode* > List;

    public:
        CollectionNode(CollectionTypes _ctype, Base* parent);

    public:
        CollectionTypes collectionType() const;
        QString collectionTypeName() const;

    private:
        ObjTypes objType() const;
        void dumpSelf(Internal::Dumper& dumper) const;
        bool refreshSelf();

    private:
        CollectionTypes ctype;
    };

}

#endif
