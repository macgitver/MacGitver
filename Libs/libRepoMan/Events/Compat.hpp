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

#include "libRepoMan/Frontend/Base.hpp"

namespace RM
{

    class REPOMAN_API CompatEventsInterface
    {
    public:
        virtual ~CompatEventsInterface() {}

    public:
        virtual void repositoryOpened(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryAboutToClose(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryActivated(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryDeactivated(const RM::Frontend::Repo& repo) = 0;

        virtual void objectCreated(const RM::Frontend::Base& object) = 0;
        virtual void objectAboutToBeDeleted(const RM::Frontend::Base& object) = 0;

        virtual void refTreeNodeCreated(const RM::Frontend::RefTreeNode& node) = 0;
        virtual void refTreeNodeAboutToBeDeleted(const RM::Frontend::RefTreeNode& node) = 0;

        virtual void refCreated(const RM::Frontend::Reference& ref) = 0;
        virtual void refAboutToBeDeleted(const RM::Frontend::Reference& ref) = 0;
        virtual void refMoved(const RM::Frontend::Reference& ref) = 0;
        virtual void refHeadDetached(const RM::Frontend::Reference& ref) = 0;

        virtual void tagCreated(const RM::Frontend::Tag& tag) = 0;
        virtual void tagAboutToBeDeleted(const RM::Frontend::Tag& tag) = 0;

        virtual void branchCreated(const RM::Frontend::Branch& branch) = 0;
        virtual void branchAboutToBeDeleted(const RM::Frontend::Branch& branch) = 0;
        virtual void branchMoved(const RM::Frontend::Branch& branch) = 0;
        virtual void branchUpstreamChanged(const RM::Frontend::Branch& branch) = 0;

        virtual void namespaceCreated(const RM::Frontend::Namespace& nameSpace) = 0;
        virtual void namespaceAboutToBeDeleted(const RM::Frontend::Namespace& nameSpace) = 0;

        virtual void refLogChanged(const RM::Frontend::RefLog& reflog) = 0;
        virtual void refLogNewEntry(const RM::Frontend::RefLog& reflog) = 0;

        virtual void stageCreated(const RM::Frontend::Reference& ref) = 0;
        virtual void stageAboutToBeDeleted(const RM::Frontend::Reference& ref) = 0;

        virtual void remoteCreated(const RM::Frontend::Remote& remote) = 0;
        virtual void remoteAboutToBeDeleted(const RM::Frontend::Remote& remote) = 0;
        virtual void remoteModified(const RM::Frontend::Remote& remote) = 0;

        virtual void submoduleCreated(const RM::Frontend::Submodule& submodule) = 0;
        virtual void submoduleAboutToBeDeleted(const RM::Frontend::Submodule& submodule) = 0;
        virtual void submoduleMoved(const RM::Frontend::Submodule& submodule) = 0;

        virtual void repositoryStateChanged(const RM::Frontend::Repo& repo) = 0;
        virtual void indexUpdated() = 0;
        virtual void workTreeUpdated() = 0;
    };

    class REPOMAN_API CompatEvents
    {
        static std::vector<CompatEventsInterface*> mEvents;

    public:
        static void addReceiver(CompatEventsInterface* ei);
        static void delReceiver(CompatEventsInterface* ei);

    public:
        static void repositoryOpened(const RM::Frontend::Repo& repo);
        static void repositoryAboutToClose(const RM::Frontend::Repo& repo);
        static void repositoryActivated(const RM::Frontend::Repo& repo);
        static void repositoryDeactivated(const RM::Frontend::Repo& repo);
        static void objectCreated(const RM::Frontend::Base& object);
        static void objectAboutToBeDeleted(const RM::Frontend::Base& object);
        static void refTreeNodeCreated(const RM::Frontend::RefTreeNode& node);
        static void refTreeNodeAboutToBeDeleted(const RM::Frontend::RefTreeNode& node);
        static void refCreated(const RM::Frontend::Reference& ref);
        static void refAboutToBeDeleted(const RM::Frontend::Reference& ref);
        static void refMoved(const RM::Frontend::Reference& ref);
        static void refHeadDetached(const RM::Frontend::Reference& ref);
        static void tagCreated(const RM::Frontend::Tag& tag);
        static void tagAboutToBeDeleted(const RM::Frontend::Tag& tag);
        static void branchCreated(const RM::Frontend::Branch& branch);
        static void branchAboutToBeDeleted(const RM::Frontend::Branch& branch);
        static void branchMoved(const RM::Frontend::Branch& branch);
        static void branchUpstreamChanged(const RM::Frontend::Branch& branch);
        static void namespaceCreated(const RM::Frontend::Namespace& nameSpace);
        static void namespaceAboutToBeDeleted(const RM::Frontend::Namespace& nameSpace);
        static void refLogChanged(const RM::Frontend::RefLog& reflog);
        static void refLogNewEntry(const RM::Frontend::RefLog& reflog);
        static void stageCreated(const RM::Frontend::Reference& ref);
        static void stageAboutToBeDeleted(const RM::Frontend::Reference& ref);
        static void remoteCreated(const RM::Frontend::Remote& remote);
        static void remoteAboutToBeDeleted(const RM::Frontend::Remote& remote);
        static void remoteModified(const RM::Frontend::Remote& remote);
        static void submoduleCreated(const RM::Frontend::Submodule& submodule);
        static void submoduleAboutToBeDeleted(const RM::Frontend::Submodule& submodule);
        static void submoduleMoved(const RM::Frontend::Submodule& submodule);
        static void repositoryStateChanged(const RM::Frontend::Repo& repo);
        static void indexUpdated();
        static void workTreeUpdated();
    };

}
