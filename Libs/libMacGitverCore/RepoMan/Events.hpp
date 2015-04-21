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

#include "libMacGitverCore/RepoMan/Core.hpp"

#include <QSet>

namespace RM
{

    namespace Frontend
    {
        class Base;
        class Namespace;
        class Repo;
        class Reference;
        class Remote;
        class Submodule;
        class RefTreeNode;
        class RefLog;
        class Branch;
        class Tag;
    }

    class MGV_CORE_API EventsInterface
    {
    public:
        virtual ~EventsInterface() {}

    public:
        virtual void repositoryOpened(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryAboutToClose(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryActivated(const RM::Frontend::Repo& repo) = 0;
        virtual void repositoryDeactivated(const RM::Frontend::Repo& repo) = 0;

        virtual void objectCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object) = 0;
        virtual void objectAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object) = 0;

        virtual void refTreeNodeCreated(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node) = 0;
        virtual void refTreeNodeAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node) = 0;

        virtual void refCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;
        virtual void refAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;
        virtual void refMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;
        virtual void refHeadDetached(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;

        virtual void tagCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag) = 0;
        virtual void tagAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag) = 0;

        virtual void branchCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch) = 0;
        virtual void branchAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch) = 0;
        virtual void branchMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch) = 0;
        virtual void branchUpstreamChanged(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch) = 0;

        virtual void namespaceCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace) = 0;
        virtual void namespaceAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace) = 0;

        virtual void refLogChanged(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog) = 0;
        virtual void refLogNewEntry(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog) = 0;

        virtual void stageCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;
        virtual void stageAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref) = 0;

        virtual void remoteCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote) = 0;
        virtual void remoteAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote) = 0;
        virtual void remoteModified(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote) = 0;

        virtual void submoduleCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule) = 0;
        virtual void submoduleAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule) = 0;
        virtual void submoduleMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule) = 0;

        virtual void repositoryStateChanged(const RM::Frontend::Repo& repo) = 0;
        virtual void indexUpdated(const RM::Frontend::Repo& repo) = 0;
        virtual void workTreeUpdated(const RM::Frontend::Repo& repo) = 0;
    };

    class MGV_CORE_API Events // : public EventsInterface
    {
    private:
        Events();

    public:
        static Events* self();
        static void addReceiver(EventsInterface* ev);
        static void delReceiver(EventsInterface* ev);

    private:
        QSet<EventsInterface*> mEvents;
        static Events* sSelf;

    public:
        void repositoryOpened(const RM::Frontend::Repo& repo);
        void repositoryAboutToClose(const RM::Frontend::Repo& repo);
        void repositoryActivated(const RM::Frontend::Repo& repo);
        void repositoryDeactivated(const RM::Frontend::Repo& repo);
        void objectCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object);
        void objectAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object);
        void refTreeNodeCreated(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node);
        void refTreeNodeAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node);
        void refCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void refAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void refMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void refHeadDetached(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void tagCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag);
        void tagAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag);
        void branchCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch);
        void branchAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch);
        void branchMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch);
        void branchUpstreamChanged(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch);
        void namespaceCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace);
        void namespaceAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace);
        void refLogChanged(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog);
        void refLogNewEntry(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog);
        void stageCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void stageAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
        void remoteCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote);
        void remoteAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote);
        void remoteModified(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote);
        void submoduleCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule);
        void submoduleAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule);
        void submoduleMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule);
        void repositoryStateChanged(const RM::Frontend::Repo& repo);
        void indexUpdated(const RM::Frontend::Repo& repo);
        void workTreeUpdated(const RM::Frontend::Repo& repo);
    };

}
