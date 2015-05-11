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

#pragma once

#include "libMacGitverCore/RepoMan/Core.hpp"

namespace RM
{

    class Repo;
    class RefTreeNode;
    class Namespace;
    class Ref;
    class Remote;
    class RefLog;
    class Submodule;
    class Tag;

    namespace Internal
    {
        class Dumper;
        class BasePrivate;
    }

    class MGV_CORE_API Base
    {
        friend class Repo;
        friend class Internal::BasePrivate;

    public:
        typedef QVector< Base* > List;

    protected:
        Base(Internal::BasePrivate& _d);
        virtual ~Base();

    public:
        ObjTypes objType() const;

    public:
        void refresh();

        const Repo* repository() const;
        Repo* repository();

        Base* parentObject() const;

        List childObjects() const;
        List childObjects(ObjTypes type) const;

        template< class T >
        typename T::List childObjects() const;

        template< class T >
        bool isA() const;

        template< class T >
        bool inheritsRepoManType() const;

        bool inheritsRepoManType(ObjTypes type) const;

        Heaven::IconRef icon(bool small = false) const;

        QString typeName() const;
        QString displayName() const;
        QString dump() const;

        Heaven::Menu* contextMenu();

    protected:
        Internal::BasePrivate* mData;

    private:
        Base(const Base& other);
        Base& operator=(const Base& other);
    };

    template< class T >
    inline bool Base::isA() const
    {
        return objType() == ObjTypes(T::StaticObjectType);
    }

    template< class T >
    inline bool Base::inheritsRepoManType() const
    {
        return inheritsRepoManType(ObjTypes(T::StaticObjectType));
    }

    template< class T >
    inline typename T::List Base::childObjects() const
    {
        typename T::List children;

        foreach(Base* child, childObjects()) {
            if (child->inheritsRepoManType<T>()) {
                children.append(static_cast<T*>(child));
            }
        }

        return children;
    }

}
