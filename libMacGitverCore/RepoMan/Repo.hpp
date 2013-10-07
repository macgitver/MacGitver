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

#ifndef MGV_REPOSITORY_INFO_HPP
#define MGV_REPOSITORY_INFO_HPP

#include <QObject>

class QTimer;

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/RepoMan/Base.hpp"

namespace RM
{

    class RepoMan;

    class MGV_CORE_API Repo : public QObject, public Base
    {
        friend class RepoMan;
        Q_OBJECT
    public:
        enum { StaticObjectType = RepoObject };
        typedef QSet< Repo* > Set;
        typedef QVector< Repo* > List;

    public:
        Repo(const Git::Repository& repo, Base* parent);
        ~Repo();

    public:
        Git::Repository gitRepo();
        Git::Repository gitLoadedRepo() const;

        bool isSubModule() const;
        bool isBare() const;
        bool isLoaded() const;
        bool isActive() const;

        Repo* parentRepository();
        List submodules() const;

        QString path() const;

        QString displayAlias() const;
        void setDisplayAlias( const QString& alias );

        QString branchDisplay() const;

        void close();

        Repo* repoByPath( const QString& basePath, bool searchSubmodules );

        Ref* findReference(const Git::Reference& ref, bool create = false);
        Namespace* findNamespace(const QStringList& _namespaces, bool create = false);
        Namespace* findNamespace(const QString& nsFullName, bool create = false);

        CollectionNode* branches();
        CollectionNode* tags();
        CollectionNode* namespaces();
        CollectionNode* notes();

    private:
        void load();
        void unload();
        void findAlias();
        bool ensureIsLoaded();
        void removeChild( Repo* child );

    private:
        // only for repoman:
        void scanSubmodules();
        void activated();
        void deactivated();

    private:
        // Base impl
        bool refreshSelf();
        void postRefreshChildren();
        ObjTypes objType() const;
        void dumpSelf(Internal::Dumper& dumper) const;

    private slots:
        void unloadTimer();

    signals:
        void aboutToClose   (RM::Repo* repo);
        void aboutToUnload  (RM::Repo* repo);
        void unloaded       (RM::Repo* repo);
        void loaded         (RM::Repo* repo);
        void childRemoved   (RM::Repo* parent, RM::Repo* child);
        void childAdded     (RM::Repo* parent, RM::Repo* child);
        void aliasChanged   (const QString& newAlias);

    private:
        Git::Repository mRepo;                  //!< GitWrap-Repo, if loaded
        QString         mPath;                  //!< Full, absolute path to this repository
        QString         mDisplayAlias;          //!< An alias for display (Default to last path comp.)
        Repo*           mParent;                //!< This subModule-Repo's parent repository
        List            mChildren;              //!< This repo's direct submodule repositories
        bool            mIsSubModule    : 1;    //!< This is a submodule of another repo
        bool            mIsBare         : 1;    //!< This is a bare repo
        bool            mIsLoaded       : 1;    //!< This repo is currently loaded (by gitWrap)
        bool            mIsActive       : 1;    //!< This is MGV's current active repo?
        QTimer*         mUnloadTimer;           //!< NULL or a timer to unload this repository
    };

}

#endif
