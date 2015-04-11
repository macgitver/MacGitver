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

#include "libRepoMan/Data/Submodule.hpp"

#include "libRepoMan/Private/Dumper.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0
        Submodule::Submodule(const Git::Repository& repo, Repo* parent)
            : Repo( *new RM::Data::Submodule( this, repo ) )
        {
            RM_D(Submodule);

            d->mIsSubModule = true;

            setDisplayAlias( repo.name() );

            d->linkToParent( parent );
            d->refresh();

            d->mIsInitializing = false;
        }
        #endif

    }

    namespace Data
    {
        #if 0

        SubmodulePrivate::SubmodulePrivate(Submodule* pub, const Git::Repository& repo)
            : RepoPrivate( pub, repo )
        {
        }

        ObjTypes SubmodulePrivate::objType() const
        {
            return SubmoduleObject;
        }

        void SubmodulePrivate::dumpSelf(Dumper& dumper) const
        {
            dumper.addLine(QString(QLatin1String("Submodule 0x%1"))
                           .arg(quintptr(mPub),0,16));
        }

        void SubmodulePrivate::postCreation()
        {
            if (!repoEventsBlocked()) {
                Events::self()->submoduleCreated(repository(), pub<Submodule>());
            }

            RepoPrivate::postCreation();
        }

        void SubmodulePrivate::preTerminate()
        {
            if (!repoEventsBlocked()) {
                Events::self()->submoduleAboutToBeDeleted(repository(), pub<Submodule>());
            }

            RepoPrivate::preTerminate();
        }

        QString SubmodulePrivate::objectTypeName() const
        {
            return QLatin1String("Submodule");
        }

        bool SubmodulePrivate::inherits(ObjTypes type) const
        {
            return type == SubmoduleObject || RepoPrivate::inherits(type);
        }

        #endif

    }

}
