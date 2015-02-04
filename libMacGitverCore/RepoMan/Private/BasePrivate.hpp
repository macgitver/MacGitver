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

#ifndef REPOMAN_BASE_PRIVATE_HPP
#define REPOMAN_BASE_PRIVATE_HPP

#include "RepoMan/Base.hpp"

#define RM_D(CLASS)     Internal::CLASS##Private* d = \
                            static_cast<Internal::CLASS##Private*>(mData)

#define RM_CD(CLASS)    const Internal::CLASS##Private* d = \
                            static_cast<const Internal::CLASS##Private*>(mData)

#define RM_P(CLASS)     CLASS* p = \
                            static_cast<CLASS*>(mPub)

#define RM_CP(CLASS)    const CLASS* p = \
                            static_cast<const CLASS*>(mPub)

namespace RM
{

    namespace Internal
    {

        class Dumper;

        class BasePrivate
        {
        protected:
            BasePrivate(Base* pub);

        public:
            virtual ~BasePrivate();

        public:
            Base*           mPub;
            BasePrivate*    mParentObj;
            Base::Set       mChildren;

        public:
            template< class T > T* pub()
            {
                return static_cast<T*>(mPub);
            }

            template< class T > const T* pub() const
            {
                return static_cast<const T*>(mPub);
            }

        public:
            void dumpRecursive(Dumper& dumper) const;

            void terminateObject();
            void linkToParent(Base* parent);
            void unlinkFromParent();
            void addChildObject(Base *object);
            void removeChildObject(Base* object);

            Repo* repository();

            void refresh();
            bool repoEventsBlocked() const;

            Base* findRefParent(const QStringList& scopes, bool create);
            RefTreeNode* findRefTreeNode(const QStringList& scopes, bool create);

            CollectionNode* getOrCreateCollection(CollectionTypes ctype) const;

            template< class T > static typename T::Private* dataOf(Base* b)
            {
                BasePrivate* p = b->mData;
                if (p->objType() != ObjTypes(T::StaticObjectType)) {
                    return NULL;
                }
                return static_cast<typename T::Private*>(p);
            }

        public:
            virtual QString displayName() const;
            virtual QString objectTypeName() const = 0;
            virtual Heaven::IconRef icon() const;
            virtual bool refreshSelf() = 0;
            virtual bool preRefresh();
            virtual void postRefresh();
            virtual void preRefreshChildren();
            virtual void postRefreshChildren();
            virtual void postCreation();
            virtual void preTerminate();
            virtual bool inherits(ObjTypes type) const;
            virtual ObjTypes objType() const = 0;
            virtual void dumpSelf(Dumper& dumper) const = 0;
        };

    }

}

#endif
