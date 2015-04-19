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

#include "libRepoMan/Frontend/Repo.hpp"

#include "libRepoMan/Events/Event.hpp"
#include "libRepoMan/Events/Compat.hpp"

#include <QObject>

namespace RM
{

    namespace Frontend { class Branch; }

    class REPOMAN_API RepoMan
            : public QObject
            , private CompatEventsInterface
    {
        Q_OBJECT
    private:
        RepoMan();
    public:
        ~RepoMan();

    public:
        void open(const QString& path);

        void closeAll();

        Frontend::Repo activeRepository();
        void activate(const Frontend::Repo& repository);

        Frontend::Repo::List repositories() const;

    public:
        static RepoMan& instance();
        void terminate();

    private slots:
        void reactivateWorkaround();

    signals:
        void firstRepositoryOpened();
        void lastRepositoryClosed();
        void repositoryClosed();
        void hasActiveRepositoryChanged(bool hasActiveRepo);

    signals:
        void repositoryOpened(const RM::Frontend::Repo& repo);
        void repositoryAboutToClose(const RM::Frontend::Repo& repo);
        void repositoryActivated(const RM::Frontend::Repo& repo);
        void repositoryDeactivated(const RM::Frontend::Repo& repo);
        void objectCreated(const RM::Frontend::Base& object);
        void objectAboutToBeDeleted(const RM::Frontend::Base& object);
        void refTreeNodeCreated(const RM::Frontend::RefTreeNode& node);
        void refTreeNodeAboutToBeDeleted(const RM::Frontend::RefTreeNode& node);
        void refCreated(const RM::Frontend::Reference& ref);
        void refAboutToBeDeleted(const RM::Frontend::Reference& ref);
        void refMoved(const RM::Frontend::Reference& ref);
        void refHeadDetached(const RM::Frontend::Reference& ref);
        void tagCreated(const RM::Frontend::Tag& tag);
        void tagAboutToBeDeleted(const RM::Frontend::Tag& tag);
        void branchCreated(const RM::Frontend::Branch& branch);
        void branchAboutToBeDeleted(const RM::Frontend::Branch& branch);
        void branchMoved(const RM::Frontend::Branch& branch);
        void branchUpstreamChanged(const RM::Frontend::Branch& branch);
        void namespaceCreated(const RM::Frontend::Namespace& nameSpace);
        void namespaceAboutToBeDeleted(const RM::Frontend::Namespace& nameSpace);
        void refLogChanged(const RM::Frontend::RefLog& reflog);
        void refLogNewEntry(const RM::Frontend::RefLog& reflog);
        void stageCreated(const RM::Frontend::Reference& ref);
        void stageAboutToBeDeleted(const RM::Frontend::Reference& ref);
        void remoteCreated(const RM::Frontend::Remote& remote);
        void remoteAboutToBeDeleted(const RM::Frontend::Remote& remote);
        void remoteModified(const RM::Frontend::Remote& remote);
        void submoduleCreated(const RM::Frontend::Submodule& submodule);
        void submoduleAboutToBeDeleted(const RM::Frontend::Submodule& submodule);
        void submoduleMoved(const RM::Frontend::Submodule& submodule);
        void repositoryStateChanged(const RM::Frontend::Repo& repo);
        void indexUpdated();
        void workTreeUpdated();
    };

}
