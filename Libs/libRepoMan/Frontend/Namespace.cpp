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

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Frontend/Namespace.hpp"

#include "libRepoMan/Private/Dumper.hpp"
#include "libRepoMan/Data/Namespace.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Namespace::Namespace(Base* _parent, const QString& _name)
            : Base(*new NamespacePrivate(this, _name))
        {
            RM_D(Namespace);
            d->linkToParent(_parent);
        }

        CollectionNode* Namespace::branches()
        {
            RM_D(Namespace);

            return d->getOrCreateCollection( ctBranches );
        }

        CollectionNode* Namespace::tags()
        {
            RM_D(Namespace);
            return d->getOrCreateCollection( ctTags );
        }

        CollectionNode* Namespace::namespaces()
        {
            RM_D(Namespace);

            return d->getOrCreateCollection( ctNamespaces );
        }

        CollectionNode* Namespace::notes()
        {
            RM_D(Namespace);

            return d->getOrCreateCollection( ctNotes );
        }

        QString Namespace::name() const
        {
            RM_D(Namespace);

            return d->name;
        }
        #endif

    }

#if 0
    //-- NamespacePrivate --------------------------------------------------------------------------

    NamespacePrivate::NamespacePrivate(Namespace* _pub, const QString& _name)
        : BasePrivate(_pub)
        , name(_name)
    {
    }

    ObjTypes NamespacePrivate::objType() const
    {
        return NamespaceObject;
    }

    void NamespacePrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QStringLiteral("Namespace 0x%1 - %2"))
                       .arg(quintptr(mPub),0,16)
                       .arg(name));
    }

    void NamespacePrivate::postCreation()
    {
        if (!repoEventsBlocked()) {
            Events::self()->namespaceCreated(repository(), pub<Namespace>());
        }

        BasePrivate::postCreation();
    }

    void NamespacePrivate::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->namespaceAboutToBeDeleted(repository(), pub<Namespace>());
        }

        BasePrivate::preTerminate();
    }

    bool NamespacePrivate::refreshSelf()
    {
        return true;
    }

    QString NamespacePrivate::displayName() const
    {
        return name;
    }

    QString NamespacePrivate::objectTypeName() const
    {
        return QStringLiteral("Namespace");
    }

    bool NamespacePrivate::inherits(ObjTypes type) const
    {
        return type == NamespaceObject || BasePrivate::inherits(type);
    }

    #endif

}
