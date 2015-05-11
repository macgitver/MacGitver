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

#include "RepoMan/Data/BaseData.hpp"
#include "RepoMan/Data/RefTreeNodeData.hpp"
#include "RepoMan/Data/CollectionNodeData.hpp"

#include "RepoMan/Base.hpp"
#include "RepoMan/Repo.hpp"

#include "RepoMan/Events.hpp"
#include "RepoMan/Private/Dumper.hpp"

#include "libHeavenIcons/IconRef.hpp"

namespace RM
{

    namespace Internal
    {

        BasePrivate::BasePrivate(Base* pub)
            : mPub(pub)
            , mRepo(NULL)
            , mParentObj(NULL)
        {
            Q_ASSERT(mPub);
        }

        BasePrivate::~BasePrivate()
        {
            // THIS _IS_ IMPORTANT
            // We forbid by definition that any RM::* object may be destroyed _before_ it is unlinked
            // from its parent. Otherwise, events cannot be triggered correctly.
            Q_ASSERT(mChildren.count() == 0);
            Q_ASSERT(mParentObj == NULL);
            Q_ASSERT(mRepo == NULL);
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
        void BasePrivate::linkToParent(Base* parent)
        {
            if (parent) {
                mParentObj = parent->mData;
                mParentObj->addChildObject(mPub);
                mRepo = searchRepository();
                refreshSelf();
                postCreation();
            }
        }

        /**
         * @internal
         * @brief       Child-Part of unlinking from the tree
         *
         * Invokes the parent part on the parent side and then cleans up the reference to the parent.
         */
        void BasePrivate::unlinkFromParent()
        {
            if (mParentObj) {
                mParentObj->removeChildObject(mPub);
                mParentObj = NULL;
                mRepo = NULL;
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
        void BasePrivate::addChildObject(Base* object)
        {
            Q_ASSERT(object);
            Q_ASSERT(!mChildren.contains(object));
            mChildren.append(object);
        }

        /**
         * @internal
         * @brief       Parent-part of unlinking a child from the parent
         *
         * @param[in]   object  The child that is to be removed from the parent
         */
        void BasePrivate::removeChildObject(Base* object)
        {
            Q_ASSERT(mChildren.contains(object));
            mChildren.removeOne(object);
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
        void BasePrivate::dumpRecursive(Dumper& dumper) const
        {
            dumpSelf(dumper);

            dumper.indent();
            foreach(Base* child, mChildren) {
                child->mData->dumpRecursive(dumper);
            }
            dumper.dedent();
        }

        QString BasePrivate::displayName() const
        {
            return QStringLiteral("<Unknown>");
        }

        /**
         * @brief       Refresh this object
         *
         * Refreshes this object and all its children. First calls to refreshSelf() expecting it to
         * update this object and send out events. If refreshSelf() returns `false`, this object is
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
        void BasePrivate::refresh()
        {
            if (!refreshSelf()) {
                // If refresh self returned false, we are no longer valid and will now destroy
                // ourselves. We just terminateObject().
                terminateObject();
                return;
            }

            postRefresh();
            preRefreshChildren();

            foreach(Base* child, mChildren) {
                child->mData->refresh();
            }

            postRefreshChildren();

            if (refreshCheckDispensable()) {
                terminateObject();
            }
        }

        void BasePrivate::postRefresh()
        {
        }

        /**
         * @brief       Check if this object is dispensable
         *
         * @return      @c true to dispense this object
         *
         * During the refresh cycle, this is the last method called for each object. If it returns
         * @c true, the object will be terminated.
         *
         * This can be used for container objects that shall actually get dispensed once they have no
         * more children (i.e. RefTreeNode).
         *
         */
        bool BasePrivate::refreshCheckDispensable()
        {
            return false;
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
        void BasePrivate::preRefreshChildren()
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
        void BasePrivate::postRefreshChildren()
        {
        }

        /**
         * @brief       Terminates the lifetime of this object
         *
         * This method MUST be used to destroy an object. Don't just delete an repoman object.
         *
         * 1. For all children, terminateObject() gets invoked.
         * 2. preTerminate() is called, whose objective is to send an event if required.
         * 3. the object is unlinkedFromParent()
         * 4. finally it deletes itself. Deleting is not defered; the object is gone immediately.
         *
         */
        void BasePrivate::terminateObject()
        {
            foreach (Base* child, mChildren) {
                child->mData->terminateObject();
            }

            preTerminate();
            unlinkFromParent();
            delete this;
        }

        /**
         * @brief       Are Events blocked for the repository this object belongs to?
         *
         * @return      `true` if any events for this repository shall be suppressed. `false` in normal
         *              operation.
         *
         * During the construction of a repository and its initial seeding with objects, no events will
         * be send to any listeners. This method must be used to query whether we're currently in that
         * phase or not.
         *
         */
        bool BasePrivate::repoEventsBlocked()
        {
            Q_ASSERT(mRepo);
            return mRepo->isInitializing();
        }

        /**
         * @brief       Pre-Termination call
         *
         * This method is called prior to doing the actual object termination. Its objective is to send
         * out any required events to listeners, unless this repository is still initializing.
         *
         * The base implementation will send out a objectAboutToDelete() event.
         */
        void BasePrivate::preTerminate()
        {
            if (!repoEventsBlocked()) {
                Events::self()->objectAboutToBeDeleted(repository(), mPub);
            }
        }

        /**
         * @brief       Post-Construction call
         *
         * This method is called just before the last constructor is finished. The call actually will
         * come from linkToParent().
         *
         * The purpose of this method is to send a creation event. The base implementation will send a
         * objectCreated() event.
         *
         * Note that no events should be sent out, if repoEventsBlocked() returns `true`.
         *
         */
        void BasePrivate::postCreation()
        {
            if (!repoEventsBlocked()) {
                Events::self()->objectCreated(repository(), mPub);
            }
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
        Base* BasePrivate::findRefParent(const QStringList& scopes, bool create)
        {
            if (scopes.isEmpty()) {
                return mPub;
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
        RefTreeNode* BasePrivate::findRefTreeNode(const QStringList &scopes, bool create)
        {
            if (scopes.isEmpty()) {
                return NULL;
            }

            Base* current = mPub;

            foreach (QString scope, scopes) {
                RefTreeNode::List nodes = current->childObjects<RefTreeNode>();
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
                        next = new RefTreeNode(current, scope);
                    }
                    else {
                        return NULL;
                    }
                }

                current = next;
            }

            return static_cast< RefTreeNode* >(current);
        }


        CollectionNode* BasePrivate::getOrCreateCollection(CollectionTypes ctype)
        {
            CollectionNode* cn;

            foreach (cn, mPub->childObjects<CollectionNode>()) {
                if (cn->collectionType() == ctype) {
                    return cn;
                }
            }

            return new CollectionNode(ctype, mPub);
        }

        Repo* BasePrivate::searchRepository()
        {
            if (!mRepo) {
                if (mParentObj) {
                    mRepo = mParentObj->repository();
                }
                else {
                    return NULL;
                }
            }
            return mRepo;
        }

        Heaven::IconRef BasePrivate::icon(bool small) const
        {
            QString size = small ? QStringLiteral("@16") : QStringLiteral("@24");
            return Heaven::IconRef::fromString(QChar(L'#') % objectTypeName() % size);
        }

        bool BasePrivate::inherits(ObjTypes type) const
        {
            return false;
        }

    }

}
