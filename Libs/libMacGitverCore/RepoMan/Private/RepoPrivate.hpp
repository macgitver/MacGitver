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

#pragma once

#include "RepoMan/Private/BasePrivate.hpp"

#include "RepoMan/Repo.hpp"

namespace RM
{

    class Head;

    namespace Internal
    {

        class RepoPrivate : public BasePrivate
        {
        public:
            RepoPrivate(Repo* pub, const Git::Repository& repo);
            ~RepoPrivate();

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            void preTerminate();
            void postRefreshChildren();
            QString displayName() const;
            void dumpSelf(Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;
            Repo* searchRepository();

        public:
            Ref*        findReference(  const Git::Reference& ref,          bool create = false);
            Ref*        findReference(  const QString& fqrn,                bool create = false);
            Remote*     findRemote(     const Git::Remote& remote,          bool create = false);
            Remote*     findRemote(     const QString& remoteName,          bool create = false);
            Namespace*  findNamespace(  const QStringList& _namespaces,     bool create = false);
            Namespace*  findNamespace(  const QString& nsFullName,          bool create = false);

        private:
            Ref* findReference(Git::RefName &rn, Git::Reference ref, bool create);

        public:
            void load();
            void unload();
            bool ensureIsLoaded();
            void findAlias();
            void scanSubmodules();
            Repo* repoByPath(const QString& basePath, bool searchSubmodules);

        public:
            Git::Repository mRepo;                 //!< GitWrap-Repo, if loaded
            QString         mPath;                 //!< Full, absolute path to this repository
            QString         mDisplayAlias;         //!< An alias for display (Default to last path comp.)
            bool            mIsSubModule    : 1;   //!< This is a submodule of another repo
            bool            mIsBare         : 1;   //!< This is a bare repo
            bool            mIsLoaded       : 1;   //!< This repo is currently loaded (by gitWrap)
            bool            mIsActive       : 1;   //!< This is MGV's current active repo?
            bool            mIsInitializing : 1;   //!< True, while this repository is initializing
            QTimer*         mUnloadTimer;          //!< NULL or a timer to unload this repository
            Head*           mHead;                 //!< The HEAD
        };

    }

}
