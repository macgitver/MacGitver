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

#include <QStack>

#include "Base.hpp"
#include "Repo.hpp"
#include "Dumper.hpp"
#include "RefTreeNode.hpp"
#include "CollectionNode.hpp"

namespace RM
{

    /**
     * @class       Base
     * @brief       Base class for all RepoMan objects
     *
     * This base class takes care of a unified linking between a parent and its children. Children
     * are always created with a pointer to their parent. Children may never be reparented. Only a
     * parent is allowed to create children for itself.
     *
     * Further, the Base class takes care of keeping the strict logic of the refresh process. For
     * details on how to implement the refresh logic, see refresh().
     *
     * Finally, this base class allows to dump a partial subtree of RepoMan objects into a textual
     * hierarchy. See dump() on advanced information.
     *
     */

    /**
     * @fn          ObjType Base::objType() const
     * @brief       Get the type of this object
     *
     * This method must be implemented by all derivats of Base. They must simply return the correct
     * value from the ObjTypes enum.
     *
     * @return      Type of this object
     *
     */

    /**
     * @fn          bool Base::refreshSelf()
     * @brief       Refresh this object's data and sent events
     *
     * This method is called during the refreshing mechanism. It is the first step and can determine
     * that the object itself does no longer exist. However, if this happens, the child refreshing
     * logic of the parent is most probably broken.
     *
     * Implementations should _only_ refresh the object itself and not the children. See refresh()
     * for details on how exactly the refreshing process works.
     */

    /**
     * @fn          bool Base::isA<T>()
     * @brief       Check this object's type
     *
     * @tparam      T   Type to check against
     *
     * @return      `true`, if this is an object of type @a T. `false` otherwise.
     */

    /*-* // Keep this comment away from doxygen: https://bugzilla.gnome.org/show_bug.cgi?id=709052
     * @fn          T::Set Base::childObjects() const
     * @brief       Find (existing) children filtered by a given type
     *
     * @tparam      T   Type to check the children against
     *
     * @return      A set of children of type @a T.
     */

    /**
     * @brief       Constructor
     *
     * Creates a new RepoMan object and links it into the parent. Because at this point the new
     * child is not yet fully constructed, no further action is taken.
     *
     * @param[in]   parent      The parent to whom we shall link this new child to.
     *
     */
    Base::Base(Base* parent)
        : mParentObj(NULL)
    {
        linkToParent(parent);
    }

    /**
     * @brief       Destructor
     *
     * At the point where the destructor is called, all children should have been unlinked from the
     * tree and this object has to be unlinked too.
     *
     */
    Base::~Base()
    {
        // THIS _IS_ IMPORTANT
        // We forbid by definition that any RM::* object may be destroyed _before_ it is unlinked
        // from its parent. Otherwise, events cannot be triggered correctly.
        Q_ASSERT(mChildren.count() == 0);
        Q_ASSERT(mParentObj == NULL);
    }

    /**
     * @brief       Child-part of linking into the tree
     *
     * @param[in]   parent  The parent to link into
     *
     * This method is called directly from the constructor. It establishes a relationship with the
     * parent object. This relationship can never be altered.
     *
     */
    void Base::linkToParent(Base* parent)
    {
        if (parent) {
            mParentObj = parent;
            mParentObj->addChildObject(this);
        }
    }

    /**
     * @internal
     * @brief       Child-Part of unlinking from the tree
     *
     * Invokes the parent part on the parent side and then cleans up the reference to the parent.
     */
    void Base::unlinkFromParent()
    {
        if (mParentObj) {
            mParentObj->removeChildObject(this);
            mParentObj = NULL;
        }
    }

    /**
     * @internal
     * @brief       Parent-part of linking a new child
     *
     * We cannot do any special processing, since the child object is not yet fully constructed. We
     * just fill the internal structure.
     *
     * @param[in]   object  The new child object that shall be linked in.
     */
    void Base::addChildObject(Base* object)
    {
        Q_ASSERT(!mChildren.contains(object));
        mChildren.insert(object);
    }

    /**
     * @internal
     * @brief       Parent-part of unlinking a child from the parent
     *
     * @param[in]   object  The child that is to be removed from the parent
     */
    void Base::removeChildObject(Base* object)
    {
        Q_ASSERT(mChildren.contains(object));
        mChildren.remove(object);
    }

    /**
     * @brief       Refresh this object
     *
     * Refreshs this object and all its children. First calls to refreshSelf() expecting it to
     * update this object and send out events. If refreshSelf() returnes `false`, this object is
     * removed from the tree. In this case all children should already have been removed from the
     * tree.
     *
     * If refreshSelf() returned `true`, preRefreshChildren() is called. It should remove children
     * that are no longer part of the tree. After that for each child, refresh() is called
     * recursively. Finally, postRefreshChildren() is invoked, which should search for new objects
     * and link them into the tree.
     *
     * If preRefreshChildren() is implemented correctly on all objects, refreshSelf() should
     * probably never have to return `false`.
     *
     */
    void Base::refresh()
    {
        preRefresh();

        if (!refreshSelf()) {
            // If refresh self returned false, we are no longer valid and will now destroy
            // ourselves. We just terminateObject().
            terminateObject();
            return;
        }

        postRefresh();
        preRefreshChildren();

        foreach(Base* child, mChildren) {
            child->refresh();
        }

        postRefreshChildren();
    }

    bool Base::preRefresh()
    {
        return true;
    }

    void Base::postRefresh()
    {
    }

    /**
     * @brief       First step in refreshing the children
     *
     * This method is called directly after the object refreshed itself (refreshSelf()) but before
     * any of its children are refreshed.
     *
     * It shall be used to figure out which children do no longer exist.
     *
     * The base implementation simply does nothing.
     *
     */
    void Base::preRefreshChildren()
    {
    }

    /**
     * @brief       Last step in refreshing the children
     *
     * This method is called as last step in the refreshing process. It shall be used to find
     * objects and add them to the tree.
     *
     * The base implementation simply does nothing.
     *
     */
    void Base::postRefreshChildren()
    {
    }

    /**
     * @brief       Find (existing) children
     *
     * @return      A set of all children of this object (unfiltered).
     */
    Base::Set Base::childObjects() const
    {
        return mChildren;
    }

    /**
     * @brief       Find (existing) children of a specific type
     *
     * @param[in]   type    The object type of the children to find.
     *
     * @return      A set of children of this object filtered by object type.
     *
     */
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

    /**
     * @brief       Get the direct parent object
     *
     * The direct parent object is specified during construction and can never be changed.
     *
     * @return      The direct parent object.
     *
     */
    Base* Base::parentObject() const
    {
        return mParentObj;
    }

    /**
     * @brief       Find the repository for this object
     *
     * Walks up the hierarchy of objects to find the repository. Since objects can never be
     * reparented, the result of this method never changes.
     *
     * @return      The first repository in hierarchy that is found
     *
     */
    const Repo* Base::repository() const
    {
        const Base* cur = this;

        while (cur) {
            if (cur->objType() == RepoObject) {
                return static_cast< const Repo* >(cur);
            }
            cur = cur->parentObject();
        }

        return NULL;
    }

    /**
     * @brief       find the repository for this object
     *
     * Walks up the hierarchy of objects to find the repository. Since objects can never be
     * reparented, the result of this method never changes.
     *
     * @return      The first repository in hierarchy that is found
     *
     */
    Repo* Base::repository()
    {
        Base* cur = this;

        while (cur) {
            if (cur->objType() == RepoObject) {
                return static_cast< Repo* >(cur);
            }
            cur = cur->parentObject();
        }

        return NULL;
    }

    /**
     * @brief       Terminates the lifetime of this object
     *
     * For all children, terminateObject() is invoked; the object is then unlinkedFromParent() and
     * finally it deletes itself.
     *
     */
    void Base::terminateObject()
    {
        foreach (Base* child, mChildren) {
            child->terminateObject();
        }

        unlinkFromParent();
        delete this;
    }

    /**
     * @brief       Creates a textual dump of this object and its children
     *
     * @return      Textual dump.
     *
     */
    QString Base::dump() const
    {
        Internal::Dumper dumper;
        dumpRecursive(dumper);
        return dumper.output();
    }

    /**
     * @internal
     * @brief       Recursively dump this object into a dumper
     *
     * All children will be dumped recursively.
     *
     * @param[in]   dumper  The dumper to output to
     *
     */
    void Base::dumpRecursive(Internal::Dumper& dumper) const
    {
        dumpSelf(dumper);

        dumper.indent();
        foreach(Base* child, mChildren) {
            child->dumpRecursive(dumper);
        }
        dumper.dedent();
    }

    /**
     * @brief       Find the parent for a Ref.
     *
     * @param[in]   scopes  List of scopes to search for or to create.
     *
     * @param[in]   create  If `true` and @a path is not empty, a reference tree node will be
     *                      created, if none is found. If `false`, `NULL` will be returned.
     *
     * @return      If @a scopes is empty, `this` is returned. Otherwise findRefTreeNode() is called
     *              to either find or create a RefTreeNode, which will be returned.
     */
    Base* Base::findRefParent(const QStringList& scopes, bool create)
    {
        if (scopes.isEmpty()) {
            return this;
        }
        return findRefTreeNode(scopes, create);
    }

    /**
     * @brief       Search for or create a ref tree node
     *
     * Searches for a RefTreeNode for path.
     *
     * @param[in]   scopes  List of scopes to search for or to create. Must not be empty.
     *
     * @param[in]   create  If `true` and a part of the tree node cannot be found, it will be
     *                      created. If `false`, `NULL` will be returned in that case.
     *
     * @return      If @a create is `true`, a valid RefTreeNode is returned. If @a create is
     *              `false`, `NULL` will be returned in case the path cannot be found.
     */
    RefTreeNode* Base::findRefTreeNode(const QStringList &scopes, bool create)
    {
        if (scopes.isEmpty()) {
            return NULL;
        }

        Base* current = this;

        foreach (QString scope, scopes) {
            RefTreeNode::Set nodes = current->childObjects<RefTreeNode>();
            RefTreeNode* next = NULL;

            foreach(RefTreeNode* child, nodes) {
                if (child->name() == scope) {
                    next = child;
                    break;
                }
            }

            if (!next) {
                if (create) {
                    // Note: We don't need to roll this back. Either we go all the way or nowhere.
                    next = new RefTreeNode(current);
                    next->setName(scope);
                }
                else {
                    return NULL;
                }
            }

            current = next;
        }

        return static_cast< RefTreeNode* >(current);
    }


    CollectionNode* Base::getOrCreateCollection(CollectionTypes ctype)
    {
        CollectionNode* cn;

        foreach (cn, childObjects<CollectionNode>()) {
            if (cn->collectionType() == ctype) {
                return cn;
            }
        }

        return new CollectionNode(ctype, this);
    }
}
