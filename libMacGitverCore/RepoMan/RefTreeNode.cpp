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

#include "RefTreeNode.hpp"
#include "Events.hpp"
#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/RefTreeNodePrivate.hpp"

namespace RM
{

    using namespace Internal;

    RefTreeNode::RefTreeNode(Base* _parent, const QString& _name)
        : Base(*new RefTreeNodePrivate(this, _name))
    {
        RM_D(RefTreeNode);
        d->linkToParent(_parent);
    }

    QString RefTreeNode::name() const
    {
        RM_D(RefTreeNode);

        return d->name;
    }

    //-- RefTreeNodePrivate ------------------------------------------------------------------------

    RefTreeNodePrivate::RefTreeNodePrivate(RefTreeNode* _pub, const QString& _name)
        : BasePrivate(_pub)
        , name(_name)
    {
    }

    ObjTypes RefTreeNodePrivate::objType() const
    {
        return RefTreeNodeObject;
    }

    void RefTreeNodePrivate::dumpSelf(Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("RefTreeNode 0x%1 [%2]"))
                       .arg(quintptr(mPub),0,16)
                       .arg(name));
    }

    void RefTreeNodePrivate::postCreation()
    {
        if (!repoEventsBlocked()) {
            Events::self()->refTreeNodeCreated(repository(), pub<RefTreeNode>());
        }

        BasePrivate::postCreation();
    }

    void RefTreeNodePrivate::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->refTreeNodeAboutToBeDeleted(repository(), pub<RefTreeNode>());
        }

        BasePrivate::preTerminate();
    }

    QString RefTreeNodePrivate::displayName() const
    {
        return name;
    }

    bool RefTreeNodePrivate::refreshSelf()
    {
        // We don't have anything to refresh - we're purely virutual
        return true;
    }

    bool RefTreeNodePrivate::refreshCheckDispensable()
    {
        return mChildren.isEmpty();
    }

    QString RefTreeNodePrivate::objectTypeName() const
    {
        return QLatin1String("RefTreeNode");
    }

    bool RefTreeNodePrivate::inherits(ObjTypes type) const
    {
        return type == RefTreeNodeObject || BasePrivate::inherits(type);
    }

}
