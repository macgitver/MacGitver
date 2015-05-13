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

#include <QDebug>
#include <QTimer>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "RepoMan/Events.hpp"
#include "RepoMan/Repo.hpp"
#include "RepoMan/RepoMan.hpp"
#include "RepoMan/Head.hpp"
#include "RepoMan/Remote.hpp"
#include "RepoMan/Namespace.hpp"
#include "RepoMan/Ref.hpp"
#include "RepoMan/Tag.hpp"
#include "RepoMan/Branch.hpp"
#include "RepoMan/Submodule.hpp"

#include "RepoMan/Data/RepoData.hpp"
#include "RepoMan/Data/RemoteData.hpp"

namespace RM
{

    namespace Frontend
    {

        Repo::Repo(Internal::RepoPrivate& _d)
            : Base(_d)
        {
        }

        Repo::Repo(const Git::Repository& repo, Base* parent)
            : Base( *new Internal::RepoPrivate(this, repo) )
        {
            RM_D(Repo);

            d->linkToParent(parent);
            d->refresh();

            d->mIsInitializing = false;
        }

        Repo::~Repo()
        {
        }

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

        QString Repo::path() const
        {
            RM_CD(Repo);

            return d->mPath;
        }

        bool Repo::isSubModule() const
        {
            RM_CD(Repo);

            return d->mIsSubModule;
        }

        bool Repo::isBare() const
        {
            RM_CD(Repo);

            return d->mIsBare;
        }

        bool Repo::isLoaded() const
        {
            RM_CD(Repo);

            return d->mIsLoaded;
        }

        bool Repo::isActive() const
        {
            RM_CD(Repo);

            return d->mIsActive;
        }

        /**
         * @brief       Are we in initialization phase?
         *
         * @return      `true` if we're still doing an initial seed with repository objects. In this
         *              case no events for new or changed objects shall be sent out to any listeners.
         *
         */
        bool Repo::isInitializing() const
        {
            RM_CD(Repo);

            return d->mIsInitializing;
        }

        Repo* Repo::parentRepository()
        {
            Base* p = parentObject();

            if (!p) {
                return NULL;
            }

            if (p->inheritsRepoManType(ObjTypes::Repo)) {
                return static_cast<Repo*>(p);
            }

            return NULL;
        }

        Repo::List Repo::submodules() const
        {
            return childObjects<Repo>();
        }

        void Repo::activated()
        {
            // ### Should be very private
            RM_D(Repo);
            Q_ASSERT(!d->mIsActive);

            if (d->mUnloadTimer) {
                d->mUnloadTimer->stop();
                d->mUnloadTimer->deleteLater();
                d->mUnloadTimer = NULL;
            }

            d->mIsActive = true;
        }

        void Repo::deactivated()
        {
            // ### Should be very private
            RM_D(Repo);
            Q_ASSERT(d->mIsActive);

            Q_ASSERT(!d->mUnloadTimer);
            d->mUnloadTimer = new QTimer(this);
            connect(d->mUnloadTimer, SIGNAL(timeout()), this, SLOT(unloadTimer()));
            d->mUnloadTimer->setInterval(15 * 60 * 1000); // quarter of an hour
            d->mUnloadTimer->start();

            d->mIsActive = false;
        }

        QString Repo::displayAlias() const
        {
            RM_CD(Repo);

            return d->mDisplayAlias;
        }

        void Repo::setDisplayAlias(const QString& alias)
        {
            RM_D(Repo);

            if (d->mDisplayAlias != alias) {
                d->mDisplayAlias = alias;
                // ###REPOMAN Create new Event "RepoAliasChanged"
                // emit aliasChanged(alias);
            }
        }

        void Repo::unloadTimer()
        {
            RM_D(Repo);
            d->unload();
        }

        void Repo::close()
        {
            RM_D(Repo);

            if (d->mIsActive) {
                MacGitver::repoMan().activate(NULL);
            }

            Events::self()->repositoryAboutToClose(this);

            foreach (Repo* child, submodules()) {
                child->close();
            }

            d->terminateObject();
        }

        QString Repo::branchDisplay() const
        {
            // TODO: This method is totally wrong placed here
            RM_D(Repo);

            if (d->mIsLoaded && d->mHead) {

                if (d->mHead->isDetached()) {
                    return tr("detached at <b>%1</b>" ).arg(d->mHead->detachedId().toString());
                }

                if (d->mHead->isUnborn()) {
                    return tr("<b style=\"color: red;\">Branch yet to be born</b>");
                }

                return tr("<b style=\"background-color: #FFB54F;"
                          "\">%1</b>" ).arg(d->mHead->symbolicName().mid(11));
            }
            return tr("&lt;unknown&gt;");
        }

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

    }

}

