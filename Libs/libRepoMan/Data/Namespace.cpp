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

#include "libRepoMan/Data/Namespace.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libRepoMan/Events.hpp"

namespace RM
{

    namespace Data
    {

        #if 0
        NamespacePrivate::NamespacePrivate(Namespace* _pub, const QString& _name)
            : BasePrivate(_pub)
            , name(_name)
        {
        }

        ObjTypes NamespacePrivate::objType() const
        {
            return ObjTypes::Namespace;
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
            return type == ObjTypes::Namespace || BasePrivate::inherits(type);
        }
        #endif

    }

}
