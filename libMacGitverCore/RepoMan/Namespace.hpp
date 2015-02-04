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

#ifndef MGV_CORE_REPOMAN_NAMESPACE_HPP
#define MGV_CORE_REPOMAN_NAMESPACE_HPP

#include "Base.hpp"
#include "CollectionNode.hpp"

namespace RM
{

    namespace Internal
    {
        class NamespacePrivate;
    }

    class MGV_CORE_API Namespace : public Base
    {
    public:
        enum { StaticObjectType = NamespaceObject };
        typedef Internal::NamespacePrivate Private;
        typedef QSet< Namespace* > Set;
        typedef QVector< Namespace* > List;

    public:
        Namespace(Base* parent, const QString& _name);

    public:
        QString name() const;

    public:
        CollectionNode* branches();
        CollectionNode* namespaces();
        CollectionNode* notes();
        CollectionNode* tags();
    };

}

#endif
