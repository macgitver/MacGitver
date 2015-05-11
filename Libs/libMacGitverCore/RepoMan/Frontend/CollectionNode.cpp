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

#include <QStringBuilder>

#include "libHeavenIcons/IconRef.hpp"

#include "RepoMan/CollectionNode.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Data/CollectionNodeData.hpp"

namespace RM
{

    using namespace Internal;

    CollectionNode::CollectionNode(CollectionTypes _ctype, Base* _parent)
        : Base(*new CollectionNodePrivate(this, _ctype))
    {
        RM_D(CollectionNode);
        d->linkToParent(_parent);
    }


    CollectionTypes CollectionNode::collectionType() const
    {
        RM_CD(CollectionNode);
        return d->ctype;
    }

    QString CollectionNode::collectionTypeName() const
    {
        RM_CD(CollectionNode);

        switch (d->ctype) {
        case ctBranches:    return tr("Branches");
        case ctTags:        return tr("Tags");
        case ctNamespaces:  return tr("Namespaces");
        case ctNotes:       return tr("Notes");
        default:            return QString();
        }
    }

    //-- CollectionNodePrivate ---------------------------------------------------------------------

    CollectionNodePrivate::CollectionNodePrivate(CollectionNode* _pub, CollectionTypes _ctype)
        : BasePrivate(_pub)
        , ctype(_ctype)
    {
    }

    ObjTypes CollectionNodePrivate::objType() const
    {
        return ObjTypes::CollectionNode;
    }

    QString CollectionNodePrivate::displayName() const
    {
        return pub<CollectionNode>()->collectionTypeName();
    }

    void CollectionNodePrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QStringLiteral("Collection '%2' 0x%1"))
                       .arg(quintptr(mPub),0,16)
                       .arg(displayName()));
    }

    bool CollectionNodePrivate::refreshSelf()
    {
        return true;
    }

    void CollectionNodePrivate::postCreation()
    {
        BasePrivate::postCreation();
    }

    void CollectionNodePrivate::preTerminate()
    {
        BasePrivate::preTerminate();
    }

    QString CollectionNodePrivate::objectTypeName() const
    {
        return QStringLiteral("CollectionNode");
    }

    Heaven::IconRef CollectionNodePrivate::icon(bool small) const
    {
        QString size = small ? QStringLiteral("@16") : QStringLiteral("@24");
        QString s;

        switch (ctype) {
        case ctBranches:    s = QStringLiteral("Branches");      break;
        case ctTags:        s = QStringLiteral("Tags");          break;
        case ctNotes:       s = QStringLiteral("Notes");         break;
        case ctNamespaces:  s = QStringLiteral("Namespaces");    break;
        default:            return Heaven::IconRef();
        }

        return Heaven::IconRef::fromString(QChar(L'#') % s % size);
    }

    bool CollectionNodePrivate::inherits(ObjTypes type) const
    {
        return type == ObjTypes::CollectionNode || BasePrivate::inherits(type);
    }

}
