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

#include "libGitWrap/Repository.hpp"

#include "libRepoMan/Frontend/Base.hpp"

namespace RM
{

    class RepoMan;
    class Head;

    namespace Data
    {
        class Repo;
    }

    namespace Frontend
    {

        class REPOMAN_API Repo
                : public Base
        {
            friend class RepoMan;
        public:
            static const ObjTypes StaticObjectType = ObjTypes::Repo;
            typedef Data::Repo DPtrType;
            typedef QVector<Repo> List;

        public:
            Repo() = default;
            Repo(const std::shared_ptr<Data::Repo>& o);
            Repo(std::shared_ptr<Data::Repo>&& o);
            ~Repo() = default;

            bool operator==(const Repo& other) const;

        public:
            GW_DEPRECATED Git::Repository gitRepo();
            GW_DEPRECATED Git::Repository gitLoadedRepo();

            bool isSubModule() const;
            bool isBare() const;
            bool isLoaded() const;
            bool isActive() const;
            bool isInitializing() const;

            Repo parentRepository();
            List submodules() const;

            QString path() const;

            QString displayAlias() const;
            void setDisplayAlias( const QString& alias );

            QString branchDisplay() const;

            void close();

            GW_DEPRECATED
            Reference   findReference(  const Git::Reference&   ref);
            Reference   findReference(  const QString&          fqrn);

            GW_DEPRECATED
            Remote      findRemote(     const Git::Remote&      remote);
            Remote      findRemote(     const QString&          remoteName);

            Namespace   findNamespace(  const QStringList&      namespaces);
            Namespace   findNamespace(  const QString&          nsFullName);

            #if 0 // ###DEAD
            Head*           head() const;
            #endif

        private:
            void activated();
            void deactivated();

        #if 0 // ###DEAD
        private slots:
            void unloadTimer();
        #endif
        };


        inline bool Repo::operator==(const Repo& other) const
        {
            return Base::operator==(other);
        }

    }

}
