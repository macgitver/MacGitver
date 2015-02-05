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

#include <QStringBuilder>

#include "libHeavenIcons/IconRef.hpp"

#include "CollectionNode.hpp"

#include "Private/Dumper.hpp"
#include "Private/CollectionNodePrivate.hpp"

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
        return CollectionNodeObject;
    }

    QString CollectionNodePrivate::displayName() const
    {
        return pub<CollectionNode>()->collectionTypeName();
    }

    void CollectionNodePrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Collection '%2' 0x%1"))
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
        return QLatin1String("CollectionNode");
    }

    Heaven::IconRef CollectionNodePrivate::icon() const
    {
        QString s;

        switch (ctype) {
        case ctBranches:    s = QLatin1String("Branches");      break;
        case ctTags:        s = QLatin1String("Tags");          break;
        case ctNotes:       s = QLatin1String("Notes");         break;
        case ctNamespaces:  s = QLatin1String("Namespaces");    break;
        default:            return Heaven::IconRef();
        }

        return Heaven::IconRef::fromString(QChar(L'#') % s % QStringLiteral("@24"));
    }

    bool CollectionNodePrivate::inherits(ObjTypes type) const
    {
        return type == CollectionNodeObject || BasePrivate::inherits(type);
    }

}
