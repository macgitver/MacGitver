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

#include "RepoMan/Private/BasePrivate.hpp"

#include "RepoMan/Repo.hpp"

namespace RM
{

    namespace Internal
    {

        class RepoPrivate : public BasePrivate
        {
        public:
            RepoPrivate(Repo* _pub, const Git::Repository& repo);
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
            void removeChild( Repo* child );
            Repo* repoByPath(const QString& basePath, bool searchSubmodules);

        public:
            Git::Repository repo;                  //!< GitWrap-Repo, if loaded
            QString         path;                  //!< Full, absolute path to this repository
            QString         displayAlias;          //!< An alias for display (Default to last path comp.)
            Repo*           parent;                //!< This subModule-Repo's parent repository
            Repo::List      children;              //!< This repo's direct submodule repositories
            bool            isSubModule    : 1;    //!< This is a submodule of another repo
            bool            isBare         : 1;    //!< This is a bare repo
            bool            isLoaded       : 1;    //!< This repo is currently loaded (by gitWrap)
            bool            isActive       : 1;    //!< This is MGV's current active repo?
            bool            isInitializing : 1;    //!< True, while this repository is initializing
            QTimer*         unloadTimer;           //!< NULL or a timer to unload this repository

        };

    }

}

