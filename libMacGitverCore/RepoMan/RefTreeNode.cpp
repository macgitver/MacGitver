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
#include "Dumper.hpp"

namespace RM
{

    RefTreeNode::RefTreeNode(Base* parent)
        : Base(parent)
    {
    }

    ObjTypes RefTreeNode::objType() const
    {
        return RefTreeNodeObject;
    }

    void RefTreeNode::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("RefTreeNode 0x%1 [%2]"))
                       .arg(quintptr(this),0,16)
                       .arg(mName));
    }

    void RefTreeNode::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->refTreeNodeAboutToBeDeleted(repository(), this);
        }
    }

    bool RefTreeNode::refreshSelf()
    {
        // We don't have anything to refresh - we're purely virutual
        return true;
    }

    void RefTreeNode::setName(const QString& name)
    {
        if (mName != name) {
            mName = name;
        }
    }

    QString RefTreeNode::name() const
    {
        return mName;
    }

}
