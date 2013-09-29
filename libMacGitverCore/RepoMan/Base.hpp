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

#ifndef MGV_CORE_REPOMAN_BASE_HPP
#define MGV_CORE_REPOMAN_BASE_HPP

#include <QObject>
#include <QVector>
#include <QSet>

#include "libMacGitverCore/MacGitverApi.hpp"

namespace RM
{

    enum ObjTypes
    {
        InvalidObject,

        RepoManagerObject,
        RepoObject,
        RefObject,
        RemoteObject,
        TagObject,
        BranchObject,
        RefTreeNodeObject,
        NamespaceObject,
        SubmoduleObject,
        RefLogObject
    };

    class Repo;

    class MGV_CORE_API Base : public QObject
    {
        Q_OBJECT
    public:
        typedef QVector< Base* > List;
        typedef QSet< Base* > Set;

    public:
        Base(Base* parent);
        ~Base();

    public:
        virtual ObjTypes objType() const = 0;

    public:
        void refresh();

        const Repo* repository() const;
        Repo* repository();

        Base* parentObject() const;

        Set childObjects() const;
        Set childObjects(ObjTypes type) const;

        template< class T >
        typename T::Set childObjects() const;

        template< class T >
        bool isA() const;

    signals:
        void aboutToVanish();
        void aboutToRemoveChild(Base* parent, Base* child);
        void aboutToAddChild(Base* parent, Base* child);

    protected:
        virtual bool refreshSelf() = 0;
        virtual void preRefreshChildren();
        virtual void postRefreshChildren();

        virtual void internalChildAdded(Base* child);
        virtual void internalRemoveChild(Base* child);

    private:
        void linkToParent(Base* parent);
        void unlinkFromParent();
        void addChildObject(Base* object);
        void removeChildObject(Base* object);

    private:
        Base*   mParentObj;
        Set     mChildren;
    };

    template< class T >
    inline bool Base::isA() const
    {
        return objType() == int(T::StaticObjectType);
    }

    template< class T >
    inline typename T::Set Base::childObjects() const
    {
        /*
        Set children = childObjects(T::StaticObjType);
        typename T::Set typedChildren;

        foreach(Base* child, children) {
            typedChildren.insert(static_cast<T*>(child));
        }

        return typedChildren;
        */
        // Equivalent, but faster:
        typename T::Set children;

        foreach(Base* child, mChildren) {
            if (child->isA<T>()) {
                children.insert(child);
            }
        }

        return children;
    }

}

#endif
