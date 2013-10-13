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

#include "libGitWrap/Result.hpp"

#include "Repo.hpp"
#include "Remote.hpp"
#include "Events.hpp"
#include "CollectionNode.hpp"
#include "Dumper.hpp"

namespace RM
{

    Remote::Remote(const Git::Remote& gitObj, Base* parent)
        : Base(parent)
    {
        mName = gitObj.name();
    }

    ObjTypes Remote::objType() const
    {
        return RemoteObject;
    }

    void Remote::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Remote %2 0x%1"))
                       .arg(quintptr(this),0,16)
                       .arg(mName));
    }

    void Remote::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->remoteAboutToBeDeleted(repository(), this);
        }
    }

    QString Remote::displayName() const
    {
        return mName;
    }

    Git::Remote Remote::gitObject()
    {
        Git::Result r;
        return repository()->gitRepo().remote(r, mName);
    }

    QString Remote::name() const
    {
        return mName;
    }

    CollectionNode* Remote::branches()
    {
        return getOrCreateCollection(ctBranches);
    }

    bool Remote::refreshSelf()
    {
        return true;
    }

}
