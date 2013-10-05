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

#include "CollectionNode.hpp"
#include "Dumper.hpp"

namespace RM
{

    CollectionNode::CollectionNode(CollectionTypes _ctype, Base* parent)
        : Base(parent)
        , ctype(_ctype)
    {
    }

    ObjTypes CollectionNode::objType() const
    {
        return CollectionNodeObject;
    }

    CollectionTypes CollectionNode::collectionType() const
    {
        return ctype;
    }

    QString CollectionNode::collectionTypeName() const
    {
        switch (ctype) {
        case ctBranches:    return tr("Branches");
        case ctTags:        return tr("Tags");
        case ctNamespaces:  return tr("Namespaces");
        case ctNotes:       return tr("Notes");
        default:            return QString();
        }
    }

    void CollectionNode::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Collection '%2' 0x%1"))
                       .arg(quintptr(this),0,16)
                       .arg(collectionTypeName()));
    }

    bool CollectionNode::refreshSelf()
    {
        return true;
    }

}
