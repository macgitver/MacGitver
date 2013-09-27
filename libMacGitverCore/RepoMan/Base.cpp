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

#include "Base.hpp"

namespace RM
{

    Base::Base(Base* parent)
        : QObject(NULL) // Don't use QObject hierarchy
        , mParentObj(NULL)
    {
        linkToParent(parent);
    }

    Base::~Base()
    {
        // THIS _IS_ IMPORTANT
        // We forbid by definition that any RM::* object may be destroyed _before_ it is unlinked
        // from its parent. Otherwise, events cannot be triggered correctly.
        Q_ASSERT(mParentObj == NULL);
    }

    void Base::linkToParent(Base* parent)
    {
        unlinkFromParent();

        if (parent) {
            mParentObj = parent;
            mParentObj->addChildObject(this);
        }
    }

    void Base::unlinkFromParent()
    {
        if (mParentObj) {
            mParentObj->removeChildObject(this);
            mParentObj = NULL;
        }
    }

    void Base::addChildObject(Base* object)
    {
        if (!mChildren.contains(object)) {
            mChildren.insert(object);
            internalChildAdded(object);
        }
    }

    void Base::removeChildObject(Base* object)
    {
        if (mChildren.contains(object)) {
            internalRemoveChild(object);
            mChildren.remove(object);
        }
    }

    void Base::internalChildAdded(Base* child)
    {
        emit aboutToAddChild(this, child);
    }

    void Base::internalRemoveChild(Base* child)
    {
        emit aboutToRemoveChild(this, child);
    }

    void Base::refresh()
    {
        if (!refreshSelf()) {
            // If refresh self returned false, we are no longer valid and will now destroy
            // ourselves. We inform whoever might be concerned via the aboutToVanish() signal and
            // then just unlink and deleteLater().
            emit aboutToVanish();
            unlinkFromParent();
            deleteLater();
            return;
        }

        preRefreshChildren();

        foreach(Base* child, mChildren) {
            child->refresh();
        }

        postRefreshChildren();
    }

    void Base::preRefreshChildren()
    {
    }

    void Base::postRefreshChildren()
    {
    }

    Base::Set Base::childObjects() const
    {
        return mChildren;
    }

    Base::Set Base::childObjects(ObjTypes type) const
    {
        Set children;

        foreach(Base* child, mChildren) {
            if (child->objType() == type) {
                children.insert(child);
            }
        }

        return children;
    }

}
