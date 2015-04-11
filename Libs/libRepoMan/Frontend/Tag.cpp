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

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Frontend/Tag.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libRepoMan/Data/Tag.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Tag::Tag(Base* _parent, const Git::Reference& _ref)
            : Ref(*new TagPrivate(this, _ref))
        {
            RM_D(Tag);
            d->linkToParent(_parent);
        }

        #endif

    }

    namespace Data
    {

        #if 0

        TagPrivate::TagPrivate(Tag* _pub, const Git::Reference& _ref)
            : RefPrivate(_pub, TagType, _ref)
        {
        }

        ObjTypes TagPrivate::objType() const
        {
            return TagObject;
        }

        void TagPrivate::postCreation()
        {
            if (!repoEventsBlocked()) {
                Events::self()->tagCreated(repository(), pub<Tag>());
            }

            RefPrivate::postCreation();
        }

        void TagPrivate::preTerminate()
        {
            if (!repoEventsBlocked()) {
                Events::self()->tagAboutToBeDeleted(repository(), pub<Tag>());
            }
            RefPrivate::preTerminate();
        }

        void TagPrivate::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QLatin1String("Tag 0x%1 - %2"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        QString TagPrivate::objectTypeName() const
        {
            return QLatin1String("Tag");
        }

        bool TagPrivate::inherits(ObjTypes type) const
        {
            return type == TagObject || RefPrivate::inherits(type);
        }
        #endif

    }

}
