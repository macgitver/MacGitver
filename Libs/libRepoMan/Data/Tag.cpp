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

#include "libRepoMan/Data/Tag.hpp"

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Private/Dumper.hpp"

namespace RM
{

    namespace Data
    {

        #if 0
        Tag::Tag(Frontend::Tag* _pub, const Git::Reference& _ref)
            : Reference(_pub, TagType, _ref)
        {
        }

        ObjTypes Tag::objType() const
        {
            return ObjTypes::Tag;
        }

        void Tag::postCreation()
        {
            if (!repoEventsBlocked()) {
                Events::self()->tagCreated(repository(), pub<Tag>());
            }

            RefPrivate::postCreation();
        }

        void Tag::preTerminate()
        {
            if (!repoEventsBlocked()) {
                Events::self()->tagAboutToBeDeleted(repository(), pub<Tag>());
            }

            RefPrivate::preTerminate();
        }

        void Tag::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Tag 0x%1 - %2"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        QString Tag::objectTypeName() const
        {
            return QStringLiteral("Tag");
        }

        bool Tag::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Tag || RefPrivate::inherits(type);
        }
        #endif

    }

}
