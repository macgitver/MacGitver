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

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/RefName.hpp"
#include "libGitWrap/Submodule.hpp"

#include "RepoMan/Events.hpp"
#include "RepoMan/RepoMan.hpp"

#include "RepoMan/Frontend/BaseInternal.hpp"
#include "RepoMan/Frontend/Repo.hpp"
#include "RepoMan/Frontend/Head.hpp"
#include "RepoMan/Frontend/Remote.hpp"
#include "RepoMan/Frontend/Namespace.hpp"
#include "RepoMan/Frontend/Reference.hpp"
#include "RepoMan/Frontend/Tag.hpp"
#include "RepoMan/Frontend/Branch.hpp"
#include "RepoMan/Frontend/Submodule.hpp"

#include "RepoMan/Data/Repo.hpp"
#include "RepoMan/Data/Head.hpp"
#include "RepoMan/Data/Remote.hpp"

#include <QDebug>

namespace RM
{

    namespace Frontend
    {

        Repo::Repo(const std::shared_ptr<Data::Repo>& d)
            : Base(std::move(std::shared_ptr<Data::Base>(d)))
        {
        }

        Repo::Repo(std::shared_ptr<Data::Repo>&& d)
            : Base(std::shared_ptr<Data::Base>(std::move(d)))
        {
        }

#if 0
        Git::Repository Repo::gitRepo()
        {
            RM_D(Repo);
            return d->gitRepo(true);
        }

        Git::Repository Repo::gitLoadedRepo()
        {
            RM_D(Repo);
            return d->gitRepo();
        }
#endif

        QString Repo::path() const
        {
            DPtrT<const Repo> d(this);
            return d->path();
        }

        bool Repo::isSubModule() const
        {
            DPtrT<const Repo> d(this);
            return d->mIsSubModule;
        }

        bool Repo::isBare() const
        {
            DPtrT<const Repo> d(this);
            return d->mIsBare;
        }

        bool Repo::isLoaded() const
        {
            DPtrT<const Repo> d(this);
            return d->mIsLoaded;
        }

        bool Repo::isActive() const
        {
            DPtrT<const Repo> d(this);
            return d && d->mIsActive;
        }

        Repo Repo::parentRepository()
        {
            DPtrT<Repo> d(this);
            return d->parent()->as<Data::Repo>();
        }

        Repo::List Repo::submodules() const
        {
            DPtrT<const Repo> d(this);
            return toFrontend<Repo>(d->submodules());
        }

        void Repo::activated()
        {
            DPtrT<Repo> d(this);

            Q_ASSERT(!d->mIsActive);

#if 0
            if (d->mUnloadTimer) {
                d->mUnloadTimer->stop();
                d->mUnloadTimer->deleteLater();
                d->mUnloadTimer = NULL;
            }
#endif
            d->mIsActive = true;
        }

        void Repo::deactivated()
        {
            DPtrT<Repo> d(this);

            Q_ASSERT(d->mIsActive);
#if 0
            Q_ASSERT(!d->mUnloadTimer);

            d->mUnloadTimer = new QTimer(this);
            connect(d->mUnloadTimer, SIGNAL(timeout()), this, SLOT(unloadTimer()));
            d->mUnloadTimer->setInterval(15 * 60 * 1000); // quarter of an hour
            d->mUnloadTimer->start();
#endif

            d->mIsActive = false;
        }

        QString Repo::displayAlias() const
        {
            DPtrT<const Repo> d(this);
            return d->mDisplayAlias;
        }

        void Repo::setDisplayAlias(const QString& alias)
        {
#if 0
            RM_D(Repo);

            if (d->mDisplayAlias != alias) {
                d->mDisplayAlias = alias;
                // ###REPOMAN Create new Event "RepoAliasChanged"
                // emit aliasChanged(alias);
            }
#endif
        }

#if 0
        void Repo::unloadTimer()
        {
            RM_D(Repo);
            d->unload();
        }
#endif

        void Repo::close()
        {
            #if 0
            RM_D(Repo);

            if (d->mIsActive) {
                MacGitver::repoMan().activate(NULL);
            }

            Events::self()->repositoryAboutToClose(this);

            foreach (Repo* child, submodules()) {
                child->close();
            }

            d->terminateObject();
            #endif
        }

        QString Repo::branchDisplay() const
        {
            // TODO: This method is totally wrong placed here
            DPtrT<const Repo> d(this);

            if (d->mIsLoaded && d->mHead) {

                if (d->mHead->isDetached()) {
                    return QObject::tr("detached at <b>%1</b>" ).arg(d->mHead->detachedId().toString());
                }

                if (d->mHead->isUnborn()) {
                    return QObject::tr("<b style=\"color: red;\">Branch yet to be born</b>");
                }

                return QObject::tr("<b style=\"background-color: #FFB54F;"
                          "\">%1</b>" ).arg(d->mHead->symbolicName().mid(11));
            }
            return QObject::tr("&lt;unknown&gt;");
        }

        #if 0
        Ref* Repo::findReference(const Git::Reference& ref)
        {
            RM_D(Repo);
            return d->findReference(ref, false);
        }

        Ref* Repo::findReference(const QString &fqrn)
        {
            RM_D(Repo);
            return d->findReference(fqrn, false);
        }

        Remote* Repo::findRemote(const Git::Remote& remote)
        {
            RM_D(Repo);
            return d->findRemote(remote, false);
        }

        Remote* Repo::findRemote(const QString& remoteName)
        {
            RM_D(Repo);
            return d->findRemote(remoteName, false);
        }

        Namespace* Repo::findNamespace(const QStringList& namespaces)
        {
            RM_D( Repo );
            return d->findNamespace(namespaces);
        }

        Namespace* Repo::findNamespace(const QString& nsFullName)
        {
            RM_D( Repo );
            return d->findNamespace(nsFullName);
        }

        Head* Repo::head() const
        {
            RM_CD(Repo);
            return d->mHead;
        }
        #endif

    }

}

