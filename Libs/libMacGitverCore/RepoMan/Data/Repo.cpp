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

#include "RepoMan/Data/RepoData.hpp"
#include "RepoMan/Data/RemoteData.hpp"

#include "RepoMan/RepoMan.hpp"
#include "RepoMan/Submodule.hpp"
#include "RepoMan/Head.hpp"
#include "RepoMan/Namespace.hpp"
#include "RepoMan/Tag.hpp"

#include "RepoMan/Private/Dumper.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/RefName.hpp"
#include "libGitWrap/Submodule.hpp"

#include <QTimer>
#include <QDebug>

namespace RM
{

    namespace Internal
    {

        RepoPrivate::RepoPrivate(Repo* pub, const Git::Repository& repo)
            : BasePrivate( pub )
        {
            mRepo = repo;
            mHead = NULL;
            mPath = mRepo.workTreePath();
            mIsLoaded = mRepo.isValid();
            mIsActive = false;
            mIsBare = mRepo.isValid() && mRepo.isBare();
            mIsSubModule = false;
            mIsInitializing = true;
            mDisplayAlias = QString();
            mUnloadTimer = NULL;

            if (mPath.endsWith(L'/')) {
                mPath = mPath.left(mPath.length() - 1);
            }

            findAlias();

            if (mDisplayAlias.isEmpty()) {
                if (mPath.isEmpty()) {
                    mDisplayAlias = Repo::tr("Unknown Repository");
                }
                else {
                    QStringList sl = mPath.split(QChar(L'/'), QString::SkipEmptyParts);
                    mDisplayAlias = sl.last();
                }
            }
        }

        RepoPrivate::~RepoPrivate()
        {
            RM_P(Repo);

            if (mIsLoaded) {
                unload();
            }

            MacGitver::repoMan().internalClosedRepo(p);
        }

        void RepoPrivate::load()
        {
            // ### Unimplemented: RepoPrivate::load()
            Q_ASSERT(!mIsLoaded);
        }

        void RepoPrivate::unload()
        {
            if (mIsActive) {
                qDebug() << "Unloading active RepoInfo. Will deactivate it first.";
                MacGitver::repoMan().activate(NULL);
            }
            Q_ASSERT(!mIsActive);

            if (mUnloadTimer) {
                mUnloadTimer->stop();
                mUnloadTimer->deleteLater();
                mUnloadTimer = NULL;
            }

            // ####REPOMAN Do we really need to send out events for Unload/Load? If yes, create
            //             real events for that!
            mIsLoaded = false;
            mRepo = Git::Repository();
        }


        QString RepoPrivate::displayName() const
        {
            return mDisplayAlias;
        }

        void RepoPrivate::preTerminate()
        {
            // Do we need to do smth?
        }

        void RepoPrivate::scanSubmodules()
        {
            RM_P(Repo);

            Git::Repository repo = gitRepo(true);
            if (!repo.isValid()) {
                return;
            }

            Git::Result r;
            Git::Submodule::List subs = repo.submodules( r );
            if (!r) {
                return;
            }

            Repo::List oldSubmodules = pub<Repo>()->submodules();

            foreach (Git::Submodule sub, subs) {
                Git::Result child;
                Git::Repository subRepo = sub.subRepository(child);
                if (!child) {
                    continue;
                }
                Q_ASSERT(subRepo.isValid());

                Repo* subInfo = NULL;
                QString path = subRepo.workTreePath();

                if (path.endsWith(L'/')) {
                    path = path.left(path.length() - 1);
                }

                subInfo = repoByPath(path, true);
                if (!subInfo) {
                    subInfo = new Submodule(subRepo, p);
                }
                else {
                    oldSubmodules.removeOne(subInfo);
                }
            }

            foreach(Repo* repo, oldSubmodules) {
                dataOf<Repo>(repo)->terminateObject();
            }
        }

        void RepoPrivate::findAlias()
        {
            // ### Unimplemented: RepoPrivate::findAlias()
        }

        bool RepoPrivate::refreshSelf()
        {
            if (!mIsLoaded) {
                // In case we're not loaded, just return and do nothing.
                return true;
            }

            if (!mHead) {
                RM_P(Repo);
                mHead = new Head(mRepo, p);
            }

            return true;
        }

        void RepoPrivate::postRefreshChildren()
        {
            if (!mIsLoaded) {
                // In case we're not loaded, just return and do nothing.
                return;
            }

            Git::Result r;
            Git::Remote::List remotes = mRepo.allRemotes(r);
            if (r) {
                foreach (const Git::Remote& remote, remotes) {
                    findRemote(remote, true);
                }
            }

            Git::ReferenceList refs = mRepo.allReferences(r);
            if (r) {
                foreach (Git::Reference ref, refs) {
                    findReference(ref, true);
                }
            }

            scanSubmodules();
        }

        /**
         * @brief       Get this object's type
         *
         * @return      always ObjTypes::Repo.
         *
         */
        ObjTypes RepoPrivate::objType() const
        {
            return ObjTypes::Repo;
        }

        void RepoPrivate::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Repository 0x%1 - %02"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mIsLoaded ? mRepo.name()
                                          : QStringLiteral("<not loaded>")));
        }


        Ref* RepoPrivate::findReference(const Git::Reference& ref, bool create)
        {
            Git::RefName rn = ref.nameAnalyzer();
            return findReference(rn, ref, create);
        }

        Ref* RepoPrivate::findReference(const QString& fqrn, bool create)
        {
            Git::RefName rn(fqrn);
            return findReference(rn, Git::Reference(), create);
        }

        Ref* RepoPrivate::findReference(Git::RefName& rn, Git::Reference ref, bool create)
        {
            return NULL;
        }

        Remote* RepoPrivate::findRemote(const QString &remoteName, bool create)
        {
            RM_P(Repo);

            foreach (Remote* remote, p->childObjects<Remote>()) {
                if (remote->name() == remoteName) {
                    return remote;
                }
            }

            if (create) {
                Git::Result r;
                Git::Remote gr = p->gitRepo().remote(r, remoteName);

                if (r && gr.isValid()) {
                    Remote* remote = new Remote(gr, p);
                    return remote;
                }
            }

            return NULL;
        }

        Remote* RepoPrivate::findRemote(const Git::Remote &remote, bool create)
        {
            RM_P( Repo );

            if (!remote.isValid()) {
                return NULL;
            }

            QString remoteName = remote.name();
            foreach (Remote* rmRemote, p->childObjects<Remote>()) {
                if (rmRemote->name() == remoteName) {
                    return rmRemote;
                }
            }

            if (create) {
                Remote* rmRemote = new Remote(remote, p);
                return rmRemote;
            }

            return NULL;
        }

        Namespace* RepoPrivate::findNamespace(const QStringList& _namespaces, bool create)
        {
            return NULL;
        }

        Namespace* RepoPrivate::findNamespace(const QString& nsFullName, bool create) {

            QStringList nsNames = nsFullName.split(QChar(L'/'));
            return findNamespace(nsNames, create);
        }

        Repo* RepoPrivate::repoByPath(const QString& basePath, bool searchSubmodules)
        {
            foreach (Repo* subRepo, mPub->childObjects<Repo>()) {

                if (subRepo->path() == basePath) {
                    return subRepo;
                }

                if (searchSubmodules) {
                    if (RepoPrivate* subRepoPriv = dataOf<Repo>(subRepo)) {
                        if (Repo* subsubRepo = subRepoPriv->repoByPath(basePath, true)) {
                            return subsubRepo;
                        }
                    }
                }
            }

            return NULL;
        }

        QString RepoPrivate::objectTypeName() const
        {
            return QStringLiteral("Repo");
        }

        bool RepoPrivate::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Repo || BasePrivate::inherits(type);
        }

        Repo* RepoPrivate::searchRepository()
        {
            return static_cast<Repo*>(mPub);
        }

        Git::Repository RepoPrivate::gitRepo(bool doLoad)
        {
            if (!mIsLoaded && doLoad) {
                load();
            }

            return mRepo;
        }

    }

}
