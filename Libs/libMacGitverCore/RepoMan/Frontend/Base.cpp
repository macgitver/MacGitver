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

#include "App/MacGitver.hpp"

#include "RepoMan/Frontend/BaseInternal.hpp"
#include "RepoMan/Frontend/Repo.hpp"
#include "RepoMan/RepoMan.hpp"
#include "RepoMan/Frontend/RefTreeNode.hpp"

#include "RepoMan/Events.hpp"

#include "RepoMan/Data/Base.hpp"

#include "RepoMan/Private/Dumper.hpp"

#include "libHeavenIcons/IconRef.hpp"

#include <QStringBuilder>
#include <QStack>

/**
 * @class       RM::Frontend::Base
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

    namespace Frontend
    {

        /**
         * @brief       Constructor
         *
         * Creates a new RepoMan object and links it into the parent. Because at this point the new
         * child is not yet fully constructed, no further action is taken.
         *
         * @param[in]   parent      The parent to whom we shall link this new child to.
         *
         */
        Base::Base(const std::shared_ptr<Data::Base>& o)
            : mData(o)
        {
        }

        Base::Base(std::shared_ptr<Data::Base>&& o)
            : mData(std::move(o))
        {
        }

        Base::Base()
        {}

        Base::Base(const Base& other)
            : mData(other.mData)
        {
        }

        Base::Base(Base&& other)
            : mData(std::move(other.mData))
        {
        }

        Base& Base::operator=(Base&& other)
        {
            std::swap(mData, other.mData);
            return * this;
        }

        bool Base::operator==(const Base& other) const
        {
            return mData == other.mData;
        }

        Base::operator bool() const
        {
            return !!mData;
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
        }

        Base& Base::operator=(const Base& other)
        {
            mData = other.mData;
            return * this;
        }

        /**
         * @brief       Get the direct parent object
         *
         * The direct parent object is specified during construction and can never be changed.
         *
         * @return      The direct parent object.
         *
         */
        Base Base::parent() const
        {
            return mData->parent();
        }

        /**
         * @brief       Find the repository for this object
         *
         * @return      The first repository in hierarchy (Repo or Submodule)
         *
         */
        Repo Base::repository() const
        {
            if (mData) {
                return mData->repository().lock();
            }
            return Repo();
        }

        bool Base::inherits(ObjTypes type) const
        {
            return mData->inherits(type);
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
            return mData->objType();
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
            mData->dumpRecursive(dumper);
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
            return mData->objectTypeName();
        }

        std::shared_ptr<RM::Data::Base> Base::data() const
        {
            return mData;
        }

        Base::List Base::children() const
        {
            // ###REPOMAN Do we need to lock here?
            if (mData) {
                //return mData->children();
            }

            return List();
        }

        /**
         * @brief       Get a string that can be used to display this object
         *
         * @return      Always `<Unknown>`. Reimplementations should return something more meaningful.
         *
         */
        QString Base::displayName() const
        {
            DPtrT<const Base> d(this);
            return d->displayName();
        }

        #if 0 // ###DEAD
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
        #endif

        /**
         * @brief       Get an icon for this object
         *
         * @return      A iconRef for this object
         *
         */
        Heaven::IconRef Base::icon(bool small) const
        {
            DPtrT<const Base> d(this);
            return d->icon(small);
        }

    }

}
