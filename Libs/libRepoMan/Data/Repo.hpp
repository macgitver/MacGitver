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

#include "libRepoMan/Data/Base.hpp"

#include "libRepoMan/Frontend/Repo.hpp"

class QTimer;

namespace RM
{

    namespace Data
    {

        class Reference;
        class Remote;
        class Namespace;
        class Head;

        class Repo
                : public Base
        {
        public:
            static const_or_constexpr ObjTypes StaticObjectType = ObjTypes::Repo;

        public:
            using FrontendT = Frontend::Repo;
            using SPtr      = std::shared_ptr<Repo>;
            using WPtr      = std::weak_ptr<Repo>;
            using SList     = std::vector<SPtr>;
            using WList     = std::vector<WPtr>;

        public:
            Repo(const Git::Repository& repo);
            ~Repo();

        public:
            ObjTypes objType() const;
            QString displayName() const;
            void dumpSelf(Internal::Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            Reference*  findReference(  const QString& fqrn,                bool create = false);

            Remote*     findRemote(     const QString& remoteName,          bool create = false);

            Namespace*  findNamespace(  const QStringList& _namespaces,     bool create = false);
            Namespace*  findNamespace(  const QString& nsFullName,          bool create = false);

        private:
            GW_DEPRECATED
            Reference* findReference(Git::RefName &rn, Git::Reference ref, bool create);

        public:
            SPtr repoByPath(const QString& basePath, bool searchSubmodules);
            WList submodules() const;

        public:
            QMutex& mutex() const;
            QString path() const;
            Git::Repository gitRepo(bool doLoad = false);

        public:
            QString         mDisplayAlias;         //!< An alias for display (Default to last path comp.)
            bool            mIsSubModule    : 1;   //!< This is a submodule of another repo
            bool            mIsBare         : 1;   //!< This is a bare repo
            bool            mIsLoaded       : 1;   //!< This repo is currently loaded (by gitWrap)
            bool            mIsActive       : 1;   //!< This is MGV's current active repo?
          //bool            mIsInitializing : 1;   //!< True, while this repository is initializing
          //QTimer*         mUnloadTimer;          //!< NULL or a timer to unload this repository
            Head*           mHead;                 //!< The HEAD

        private:
            Git::Repository mRepo;                 //!< GitWrap-Repo, if loaded
            QString         mPath;                 //!< Full, absolute path to this repository
            mutable QMutex  mMutex;                //!< One mutex to protect them all (inside this Repo)
            SList           mSubmodules;
        };

        inline QMutex& Repo::mutex() const
        {
            return mMutex;
        }

        inline Repo::WList Repo::submodules() const
        {
            return weakFromSharedList<Repo>(mSubmodules);
        }

        inline QString Repo::path() const
        {
            return mPath;
        }

    }

}
