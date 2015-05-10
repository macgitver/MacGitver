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

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/RefName.hpp"
#include "libGitWrap/Submodule.hpp"

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

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/RepoPrivate.hpp"
#include "RepoMan/Private/RemotePrivate.hpp"
#include "RepoMan/Private/CollectionNodePrivate.hpp"

namespace RM
{

    using namespace Internal;

    Repo::Repo(RepoPrivate& _d)
        : Base(_d)
    {
    }

    Repo::Repo(const Git::Repository& repo, Base* parent)
        : Base( *new RepoPrivate(this, repo) )
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

        if (d->ensureIsLoaded()) {
            return d->mRepo;
        }

        return Git::Repository();
    }

    Git::Repository Repo::gitLoadedRepo() const
    {
        RM_CD(Repo);

        return d->mIsLoaded ? d->mRepo : Git::Repository();
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

        if (p->inheritsRepoManType(RepoObject)) {
            return static_cast<Repo*>(p);
        }

        return NULL;
    }

    Repo::Set Repo::submodules() const
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
            emit aliasChanged(alias);
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
        emit aboutToClose(this);

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

    /**
     * @brief       Get this repository's collection of branches
     *
     * @return      A CollectionNode whose children are the branches included in this repository.
     *
     * Branches are references matching the regular expression `^refs/heads/.*$`. Branches may be
     * scoped in which case they are subdivided into RefTreeNode objects.
     *
     */
    CollectionNode* Repo::branches()
    {
        RM_D( Repo );
        return d->getOrCreateCollection( ctBranches );
    }

    /**
     * @brief       Get this repository's collection of tags
     *
     * @return      A CollectionNode whose children are the tags included in this repository.
     *
     * Tags are references matching the regular expression `^refs/tags/.*$`. Tags may be scoped in
     * which case they are subdivided into RefTreeNode objects.
     *
     */
    CollectionNode* Repo::tags()
    {
        RM_D( Repo );
        return d->getOrCreateCollection( ctTags );
    }

    /**
     * @brief       Get this repository's collection of namespaces
     *
     * @return      A CollectionNode whose children are the 1st level namespaces included in this
     *              repository.
     *
     */
    CollectionNode* Repo::namespaces()
    {
        RM_D(Repo);
        return d->getOrCreateCollection(ctNamespaces);
    }

    /**
     * @brief       Get this repository's collection of notes
     *
     * @return      A CollectionNode object whose children are the notes included in this repository.
     *
     * Notes are refs matching the regular expression `^refs/notes/.*$`. Notes may be scoped, in
     * which case they are subdivided into RefTreeNode objects.
     *
     */
    CollectionNode* Repo::notes()
    {
        RM_D(Repo);
        return d->getOrCreateCollection(ctNotes);
    }

    //-- RepoPrivate -------------------------------------------------------------------------------

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

    bool RepoPrivate::ensureIsLoaded()
    {
        if (!mIsLoaded) {
            load();
        }

        return mIsLoaded;
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

        Repo* r = pub<Repo>();
        emit r->aboutToUnload(r);

        mIsLoaded = false;
        mRepo = Git::Repository();

        emit r->unloaded(r);
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

        if (!ensureIsLoaded()) {
            return;
        }

        Git::Result r;
        Git::Submodule::List subs = mRepo.submodules( r );
        if (!r) {
            return;
        }

        Repo::Set oldSubmodules = pub<Repo>()->submodules();

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
                emit p->childAdded(p, subInfo);

            }
            else {
                oldSubmodules.remove(subInfo);
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
     * @return      always RepoObject.
     *
     */
    ObjTypes RepoPrivate::objType() const
    {
        return RepoObject;
    }

    void RepoPrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QStringLiteral("Repository 0x%1 - %02"))
                       .arg(quintptr(mPub),0,16)
                       .arg(mIsLoaded ? pub<Repo>()->gitLoadedRepo().name()
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
        Base* parent = NULL;
        CollectionNode* cn = NULL;

        if (rn.isRemote() && rn.isBranch()) {
            Remote* rm = findRemote(rn.remote(), create);

            if (!rm) {
                return NULL;
            }

            RemotePrivate* rmp = dataOf<Remote>(rm);
            if (!rmp) {
                return NULL;
            }

            parent = rmp->findRefParent(rn.scopes(), create);
        }
        else if (rn.isNamespaced()) {
            Namespace* ns = findNamespace(rn.namespaces(), create);

            if (!ns) {
                return NULL;
            }

            if (rn.isBranch()) {
                cn = ns->branches();
            }
            else if (rn.isTag()) {
                cn = ns->tags();
            }
            else {
                return NULL;
            }

            CollectionNodePrivate* cnp = dataOf<CollectionNode>(cn);
            if (!cnp) {
                return NULL;
            }

            parent = cnp->findRefParent(rn.scopes(), create);
        }
        else {
            if (rn.isBranch()) {
                cn = getOrCreateCollection(ctBranches);
            }
            else if (rn.isTag()) {
                cn = getOrCreateCollection(ctTags);
            }
            else {
                return NULL;
            }

            CollectionNodePrivate* cnp = dataOf<CollectionNode>(cn);
            if (!cnp) {
                return NULL;
            }

            parent = cnp->findRefParent(rn.scopes(), create);
        }

        foreach (Ref* rmRef, parent->childObjects<Ref>()) {
            if (rmRef->fullName() == rn.fullName()) {
                return rmRef;
            }
        }

        if (create) {
            if ( rn.isBranch() ) {
                return new Branch( parent, ref );
            }
            else if( rn.isTag() ) {
                return new Tag( parent, ref );
            }
            else {
                qDebug() << "Do not know how to deal with reference:" << rn.fullName();
                return NULL;
            }
        }

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

    Namespace* RepoPrivate::findNamespace(const QStringList& _namespaces, bool create) {

        Base* par = getOrCreateCollection(ctNamespaces);
        Namespace* child = NULL;

        foreach (QString nsName, _namespaces) {
            child = NULL;

            foreach (Namespace* ns, par->childObjects<Namespace>()) {
                if (ns->name() == nsName) {
                    child = ns;
                    break;
                }
            }

            if (!child) {
                if (create) {
                    child = new Namespace(par, nsName);
                }
                else {
                    return NULL;
                }
            }

            par = child;
        }

        return child;
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
        return type == RepoObject || BasePrivate::inherits(type);
    }

    Repo* RepoPrivate::searchRepository()
    {
        return static_cast<Repo*>(mPub);
    }
}
