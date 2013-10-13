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

#include "Namespace.hpp"
#include "Events.hpp"
#include "Dumper.hpp"

namespace RM
{

    Namespace::Namespace(Base* parent, const QString& _name)
        : Base(parent)
        , mName(_name)
    {
    }

    ObjTypes Namespace::objType() const
    {
        return NamespaceObject;
    }

    void Namespace::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Namespace 0x%1 - %2"))
                       .arg(quintptr(this),0,16)
                       .arg(mName));
    }

    void Namespace::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->namespaceAboutToBeDeleted(repository(), this);
        }
    }

    bool Namespace::refreshSelf()
    {
        return true;
    }

    CollectionNode* Namespace::branches()
    {
        return getOrCreateCollection(ctBranches);
    }

    CollectionNode* Namespace::tags()
    {
        return getOrCreateCollection(ctTags);
    }

    CollectionNode* Namespace::namespaces()
    {
        return getOrCreateCollection(ctNamespaces);
    }

    CollectionNode* Namespace::notes()
    {
        return getOrCreateCollection(ctNotes);
    }

    QString Namespace::displayName() const
    {
        return mName;
    }

    QString Namespace::name() const
    {
        return mName;
    }

}
