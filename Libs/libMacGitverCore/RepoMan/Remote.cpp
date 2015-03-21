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

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/RemotePrivate.hpp"

namespace RM
{

    using namespace Internal;

    Remote::Remote(const Git::Remote& gitObj, Base* _parent)
        : Base(*new RemotePrivate(this, gitObj))
    {
        RM_D(Remote);

        d->linkToParent(_parent);
    }

    Git::Remote Remote::gitObject()
    {
        Git::Result r;
        return repository()->gitRepo().remote(r, name());
    }

    QString Remote::name() const
    {
        RM_D(Remote);

        return d->name;
    }

    CollectionNode* Remote::branches()
    {
        RM_D(Remote);


        // ### We don't use this when filling up the branches, yet.
        return d->getOrCreateCollection(ctBranches);
    }

    //-- RemotePrivate -----------------------------------------------------------------------------

    RemotePrivate::RemotePrivate(Remote* _pub, const Git::Remote& _obj)
        : BasePrivate(_pub)
    {
        name = _obj.name();
    }

    ObjTypes RemotePrivate::objType() const
    {
        return RemoteObject;
    }

    void RemotePrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Remote %2 0x%1"))
                       .arg(quintptr(mPub),0,16)
                       .arg(name));
    }

    void RemotePrivate::postCreation()
    {
        if (!repoEventsBlocked()) {
            Events::self()->remoteCreated(repository(), pub<Remote>());
        }

        BasePrivate::postCreation();
    }

    void RemotePrivate::preTerminate()
    {
        if (!repoEventsBlocked()) {
            Events::self()->remoteAboutToBeDeleted(repository(), pub<Remote>());
        }

        BasePrivate::preTerminate();
    }

    QString RemotePrivate::displayName() const
    {
        return name;
    }


    bool RemotePrivate::refreshSelf()
    {
        return true;
    }

    QString RemotePrivate::objectTypeName() const
    {
        return QLatin1String("Remote");
    }

    bool RemotePrivate::inherits(ObjTypes type) const
    {
        return type == RemoteObject || BasePrivate::inherits(type);
    }

}
