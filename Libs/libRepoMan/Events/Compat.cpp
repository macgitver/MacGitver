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

#include "libRepoMan/Events/Event.hpp"
#include "libRepoMan/Events/Compat.hpp"

namespace RM
{

    /**
     * @class       CompatEventsInterface
     * @brief       Interface that receives all RepoMan events
     *
     * All pure virtual methods in this interface represent events from the RepoMan.
     *
     * Each of these events can be connected to individually via Qt's SIGNAL/SLOT mechanism on the
     * MacGitver::repoMan() singleton.
     *
     * You can also write an implementation of the whole interface and register it through the
     * CompatEvents singleton. We're doing that in the unit tests - and probably will do it in a
     * Heaven::View in order to display the repository events.
     *
     * @fn          CompatEventsInterface::repositoryOpened(Repo* repo)
     * @brief       Sent after a repository is opened
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that was opened
     *
     *
     * @fn          CompatEventsInterface::repositoryAboutToClose(Repo* repo)
     * @brief       Sent before a repository will be closed
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that is about to be closed
     *
     *
     * @fn          CompatEventsInterface::repositoryActivated(Repo* repo)
     * @brief       Sent after a repository was activated
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that became the active one
     *
     *
     * @fn          CompatEventsInterface::repositoryDeactivated(Repo* repo)
     * @brief       Sent after a repository was deactivated
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that was activated
     *
     *
     * @fn          CompatEventsInterface::repositoryStateChanged(Repo* repo)
     * @brief       Sent after a change in a repository's state was detected
     *
     * Transitions between "normal", "rebasing", "merging" etc. are detected.
     *
     * @param[in]   repo    The repository whose state changed
     *
     * @fn          CompatEventsInterface::refTreeNodeCreated(Repo* repo, RefTreeNode* node)
     * @brief       Sent when a new tree node in a ref tree was created
     *
     * This is a virtual event. It is artificially created.
     *
     * @param[in]   repo    The repository for which a ref tree node was created
     * @param[in]   node    The node that was created
     *
     *
     * @fn          CompatEventsInterface::refTreeNodeAboutToBeRemoved(Repo* repo, RefTreeNode* node)
     * @brief       Sent before a tree node in a ref tree will be removed
     *
     * This is a virtual event. It is artificially created.
     *
     * @param[in]   repo    The repository for which a ref tree node will be removed
     * @param[in]   node    The node that will be removed
     *
     *
     * @fn          CompatEventsInterface::refCreated(Repo* repo, Ref* ref)
     * @brief       Sent after a reference was created
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The newly created reference
     *
     * @fn          CompatEventsInterface::refAboutToBeRemoved(Repo* repo, Ref* ref)
     * @brief       Sent before a reference is about to be removed
     *
     * The reference is most probably already physically removed. This event is sent before it is
     * removed from RepoMan's data model.
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The reference that will be removed
     *
     *
     * @fn          CompatEventsInterface::refMoved(Repo* repo, Ref* ref)
     * @brief       Sent when either the target id or the symbolic target of a reference changed.
     *
     * The reference is most probably already physically moved. This event is sent when the change
     * is discovered by the RepoMan.
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The reference that will move
     *
     *
     * @fn          CompatEventsInterface::refHeadDetached(Repo* repo, Ref* ref)
     * @brief       Sent after the HEAD reference got into a detached state
     *
     * This event will be sent when RepoMan detected that the HEAD reference has been set to a
     * detached state.
     *
     * This will happen constantly during a rebase action and also happens after checking out a
     * given SHA1 or a tag.
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The HEAD reference that got detached
     *
     */

    std::vector<CompatEventsInterface*> CompatEvents::mEvents;

    void CompatEvents::addReceiver(CompatEventsInterface* ev)
    {
        mEvents.push_back(ev);
    }

    void CompatEvents::delReceiver(CompatEventsInterface* ev)
    {
        for (auto it = mEvents.begin(); it != mEvents.end(); ++it) {
            if (*it == ev) {
                mEvents.erase(it);
                return;
            }
        }
    }

    void CompatEvents::repositoryOpened(const RM::Frontend::Repo& repo)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->repositoryOpened(repo);
        }
    }

    void CompatEvents::repositoryAboutToClose(const RM::Frontend::Repo& repo)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->repositoryAboutToClose(repo);
        }
    }

    void CompatEvents::repositoryActivated(const RM::Frontend::Repo& repo)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->repositoryActivated(repo);
        }
    }

    void CompatEvents::repositoryDeactivated(const RM::Frontend::Repo& repo)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->repositoryDeactivated(repo);
        }
    }

    void CompatEvents::objectCreated(const RM::Frontend::Base& object)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->objectCreated(object);
        }
    }

    void CompatEvents::objectAboutToBeDeleted(const RM::Frontend::Base& object)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->objectAboutToBeDeleted(object);
        }
    }

    void CompatEvents::refTreeNodeCreated(const RM::Frontend::RefTreeNode& node)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refTreeNodeCreated(node);
        }
    }

    void CompatEvents::refTreeNodeAboutToBeDeleted(const RM::Frontend::RefTreeNode& node)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refTreeNodeAboutToBeDeleted(node);
        }
    }

    void CompatEvents::refCreated(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refCreated(ref);
        }
    }

    void CompatEvents::refAboutToBeDeleted(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refAboutToBeDeleted(ref);
        }
    }

    void CompatEvents::refMoved(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refMoved(ref);
        }
    }

    void CompatEvents::refHeadDetached(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refHeadDetached(ref);
        }
    }

    void CompatEvents::tagCreated(const RM::Frontend::Tag& tag)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->tagCreated(tag);
        }
    }

    void CompatEvents::tagAboutToBeDeleted(const RM::Frontend::Tag& tag)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->tagAboutToBeDeleted(tag);
        }
    }

    void CompatEvents::branchCreated(const RM::Frontend::Branch& branch)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->branchCreated(branch);
        }
    }

    void CompatEvents::branchAboutToBeDeleted(const RM::Frontend::Branch& branch)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->branchAboutToBeDeleted(branch);
        }
    }

    void CompatEvents::branchMoved(const RM::Frontend::Branch& branch)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->branchMoved(branch);
        }
    }

    void CompatEvents::branchUpstreamChanged(const RM::Frontend::Branch& branch)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->branchUpstreamChanged(branch);
        }
    }

    void CompatEvents::namespaceCreated(const RM::Frontend::Namespace& nameSpace)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->namespaceCreated(nameSpace);
        }
    }

    void CompatEvents::namespaceAboutToBeDeleted(const RM::Frontend::Namespace& nameSpace)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->namespaceAboutToBeDeleted(nameSpace);
        }
    }

    void CompatEvents::refLogChanged(const RM::Frontend::RefLog& reflog)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refLogChanged(reflog);
        }
    }

    void CompatEvents::refLogNewEntry(const RM::Frontend::RefLog& reflog)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->refLogNewEntry(reflog);
        }
    }

    void CompatEvents::stageCreated(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->stageCreated(ref);
        }
    }

    void CompatEvents::stageAboutToBeDeleted(const RM::Frontend::Reference& ref)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->stageAboutToBeDeleted(ref);
        }
    }

    void CompatEvents::remoteCreated(const RM::Frontend::Remote& remote)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->remoteCreated(remote);
        }
    }

    void CompatEvents::remoteAboutToBeDeleted(const RM::Frontend::Remote& remote)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->remoteAboutToBeDeleted(remote);
        }
    }

    void CompatEvents::remoteModified(const RM::Frontend::Remote& remote)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->remoteModified(remote);
        }
    }

    void CompatEvents::submoduleCreated(const RM::Frontend::Submodule& submodule)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->submoduleCreated(submodule);
        }
    }

    void CompatEvents::submoduleAboutToBeDeleted(const RM::Frontend::Submodule& submodule)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->submoduleAboutToBeDeleted(submodule);
        }
    }

    void CompatEvents::submoduleMoved(const RM::Frontend::Submodule& submodule)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->submoduleMoved(submodule);
        }
    }

    void CompatEvents::repositoryStateChanged(const RM::Frontend::Repo& repo)
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->repositoryStateChanged(repo);
        }
    }

    void CompatEvents::indexUpdated()
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->indexUpdated();
        }
    }

    void CompatEvents::workTreeUpdated()
    {
        for (CompatEventsInterface* ei : mEvents) {
            ei->workTreeUpdated();
        }
    }

}
