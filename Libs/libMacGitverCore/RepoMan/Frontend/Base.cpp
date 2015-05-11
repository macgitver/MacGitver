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

#include <QStringBuilder>
#include <QStack>

#include "libHeavenIcons/IconRef.hpp"

#include "App/MacGitver.hpp"

#include "RepoMan/Base.hpp"
#include "RepoMan/Repo.hpp"
#include "RepoMan/RepoMan.hpp"
#include "RepoMan/RefTreeNode.hpp"
#include "RepoMan/CollectionNode.hpp"
#include "RepoMan/Events.hpp"

#include "RepoMan/Data/RepoManData.hpp"
#include "RepoMan/Data/BaseData.hpp"

#include "RepoMan/Private/Dumper.hpp"

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

namespace RM
{

    using namespace Internal;

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
    Base::Base(BasePrivate& _d)
        : mData(&_d)
    {
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
        delete mData;
    }

    /**
     * @brief       Find (existing) children
     *
     * @return      A set of all children of this object (unfiltered).
     */
    Base::List Base::childObjects() const
    {
        RM_CD(Base);

        return d->mChildren;
    }

    /**
     * @brief       Find (existing) children of a specific type
     *
     * @param[in]   type    The object type of the children to find.
     *
     * @return      A set of children of this object filtered by object type.
     *
     */
    Base::List Base::childObjects(ObjTypes type) const
    {
        RM_CD(Base);

        List children;

        foreach(Base* child, d->mChildren) {
            if (child->objType() == type) {
                children.append(child);
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
        RM_CD(Base);

        return d->mParentObj->mPub;
    }

    /**
     * @brief       Refresh this object
     *
     * Refreshs this object and all its children.
     *
     */
    void Base::refresh()
    {
        RM_D(Base);
        d->refresh();
    }

    /**
     * @brief       Find the repository for this object
     *
     * @return      The first repository in hierarchy (Repo or Submodule)
     *
     */
    const Repo* Base::repository() const
    {
        RM_CD(Base);
        return d->mRepo;
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
        RM_D(Base);
        return d->mRepo;
    }

    /**
     * @brief       Get a string that can be used to display this object
     *
     * @return      Always `<Unknown>`. Reimplementations should return something more meaningful.
     *
     */
    QString Base::displayName() const
    {
        RM_CD(Base);
        return d->displayName();
    }

    /**
     * @brief       Get the type of this object
     *
     * This method must be implemented by all derivats of Base. They must simply return the correct
     * value from the ObjTypes enum.
     *
     * @return      Type of this object
     *
     */
    ObjTypes Base::objType() const
    {
        RM_CD(Base);
        return d->objType();
    }

    /**
     * @brief       Creates a textual dump of this object and its children
     *
     * @return      Textual dump.
     *
     */
    QString Base::dump() const
    {
        RM_CD(Base);

        Dumper dumper;
        d->dumpRecursive(dumper);
        return dumper.output();
    }

    /**
     * @brief       Get the name of this object type
     *
     * @return      The name
     *
     */
    QString Base::typeName() const
    {
        RM_CD(Base);
        return d->objectTypeName();
    }

    /**
     * @brief       Get a context menu for this object
     *
     * @return      A Heaven::Menu that can be used as context menu for this object.
     *
     */
    Heaven::Menu* Base::contextMenu()
    {
        RepoMan* rm = &MacGitver::repoMan();
        RepoMan::Private* rmp = BasePrivate::dataOf<RepoMan>(rm);
        return rmp->contextMenuFor(this);
    }

    /**
     * @brief       Get an icon for this object
     *
     * @return      A iconRef for this object
     *
     */
    Heaven::IconRef Base::icon(bool small) const
    {
        RM_D(Base);
        return d->icon(small);
    }

    bool Base::inheritsRepoManType(ObjTypes type) const
    {
        RM_CD(Base);
        return d->inherits(type);
    }

}
