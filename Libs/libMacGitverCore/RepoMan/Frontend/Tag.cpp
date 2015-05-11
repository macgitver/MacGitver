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

#include "RepoMan/Events.hpp"

#include "RepoMan/Tag.hpp"

#include "RepoMan/Private/Dumper.hpp"

#include "RepoMan/Data/TagData.hpp"

namespace RM
{

    using namespace Internal;

    Tag::Tag(Base* _parent, const Git::Reference& _ref)
        : Ref(*new TagPrivate(this, _ref))
    {
        RM_D(Tag);
        d->linkToParent(_parent);
    }

    //-- TagPrivate --------------------------------------------------------------------------------

    TagPrivate::TagPrivate(Tag* _pub, const Git::Reference& _ref)
        : RefPrivate(_pub, TagType, _ref)
    {
    }

    ObjTypes TagPrivate::objType() const
    {
        return ObjTypes::Tag;
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
        dumper.addLine(QString(QStringLiteral("Tag 0x%1 - %2"))
                       .arg(quintptr(mPub),0,16)
                       .arg(mName));
    }

    QString TagPrivate::objectTypeName() const
    {
        return QStringLiteral("Tag");
    }

    bool TagPrivate::inherits(ObjTypes type) const
    {
        return type == ObjTypes::Tag || RefPrivate::inherits(type);
    }

}
