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

#include "RepoMan/Events.hpp"

namespace RM
{

    /**
     * @class       EventsInterface
     * @brief       Interface that receives all RepoMan events
     *
     * All pure virtual methods in this interface represent events from the RepoMan.
     *
     * Each of these events can be connected to individually via Qt's SIGNAL/SLOT mechanism on the
     * MacGitver::repoMan() singleton.
     *
     * You can also write an implementation of the whole interface and register it through the
     * Events singleton. We're doing that in the unit tests - and probably will do it in a
     * Heaven::View in order to display the repository events.
     *
     * @fn          EventsInterface::repositoryOpened(Repo* repo)
     * @brief       Sent after a repository is opened
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that was opened
     *
     *
     * @fn          EventsInterface::repositoryAboutToClose(Repo* repo)
     * @brief       Sent before a repository will be closed
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that is about to be closed
     *
     *
     * @fn          EventsInterface::repositoryActivated(Repo* repo)
     * @brief       Sent after a repository was activated
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that became the active one
     *
     *
     * @fn          EventsInterface::repositoryDeactivated(Repo* repo)
     * @brief       Sent after a repository was deactivated
     *
     * This is an administrative event. It implies no actual change to the underlying repository.
     *
     * @param[in]   repo    The repository that was activated
     *
     *
     * @fn          EventsInterface::repositoryStateChanged(Repo* repo)
     * @brief       Sent after a change in a repository's state was detected
     *
     * Transitions between "normal", "rebasing", "merging" etc. are detected.
     *
     * @param[in]   repo    The repository whose state changed
     *
     * @fn          EventsInterface::refTreeNodeCreated(Repo* repo, RefTreeNode* node)
     * @brief       Sent when a new tree node in a ref tree was created
     *
     * This is a virtual event. It is artificially created.
     *
     * @param[in]   repo    The repository for which a ref tree node was created
     * @param[in]   node    The node that was created
     *
     *
     * @fn          EventsInterface::refTreeNodeAboutToBeRemoved(Repo* repo, RefTreeNode* node)
     * @brief       Sent before a tree node in a ref tree will be removed
     *
     * This is a virtual event. It is artificially created.
     *
     * @param[in]   repo    The repository for which a ref tree node will be removed
     * @param[in]   node    The node that will be removed
     *
     *
     * @fn          EventsInterface::refCreated(Repo* repo, Ref* ref)
     * @brief       Sent after a reference was created
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The newly created reference
     *
     * @fn          EventsInterface::refAboutToBeRemoved(Repo* repo, Ref* ref)
     * @brief       Sent before a reference is about to be removed
     *
     * The reference is most probably already physically removed. This event is sent before it is
     * removed from RepoMan's data model.
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The reference that will be removed
     *
     *
     * @fn          EventsInterface::refMoved(Repo* repo, Ref* ref)
     * @brief       Sent when either the target id or the symbolic target of a reference changed.
     *
     * The reference is most probably already physically moved. This event is sent when the change
     * is discovered by the RepoMan.
     *
     * @param[in]   repo    The affected repository
     * @param[in]   ref     The reference that will move
     *
     *
     * @fn          EventsInterface::refHeadDetached(Repo* repo, Ref* ref)
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

    Events::Events()
    {
    }

    Events* Events::sSelf = NULL;

    Events* Events::self()
    {
        if (sSelf == NULL) {
            sSelf = new Events;
        }
        return sSelf;
    }

    void Events::addReceiver(EventsInterface* ev)
    {
        self()->mEvents.insert(ev);
    }

    void Events::delReceiver(EventsInterface* ev)
    {
        self()->mEvents.remove(ev);
    }

    void Events::repositoryOpened(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryOpened(repo);
        }
    }

    void Events::repositoryAboutToClose(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryAboutToClose(repo);
        }
    }

    void Events::repositoryActivated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryActivated(repo);
        }
    }

    void Events::repositoryDeactivated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryDeactivated(repo);
        }
    }

    void Events::objectCreated(Repo* repo, Base* object)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->objectCreated(repo, object);
        }
    }

    void Events::objectAboutToBeDeleted(Repo* repo, Base* object)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->objectAboutToBeDeleted(repo, object);
        }
    }

    void Events::refTreeNodeCreated(Repo* repo, RefTreeNode* node)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refTreeNodeCreated(repo, node);
        }
    }

    void Events::refTreeNodeAboutToBeDeleted(Repo* repo, RefTreeNode* node)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refTreeNodeAboutToBeDeleted(repo, node);
        }
    }

    void Events::refCreated(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refCreated(repo, ref);
        }
    }

    void Events::refAboutToBeDeleted(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refAboutToBeDeleted(repo, ref);
        }
    }

    void Events::refMoved(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refMoved(repo, ref);
        }
    }

    void Events::refHeadDetached(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refHeadDetached(repo, ref);
        }
    }

    void Events::tagCreated(Repo* repo, Tag* tag)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->tagCreated(repo, tag);
        }
    }

    void Events::tagAboutToBeDeleted(Repo* repo, Tag* tag)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->tagAboutToBeDeleted(repo, tag);
        }
    }

    void Events::branchCreated(Repo* repo, Branch* branch)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->branchCreated(repo, branch);
        }
    }

    void Events::branchAboutToBeDeleted(Repo* repo, Branch* branch)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->branchAboutToBeDeleted(repo, branch);
        }
    }

    void Events::branchMoved(Repo* repo, Branch* branch)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->branchMoved(repo, branch);
        }
    }

    void Events::branchUpstreamChanged(Repo* repo, Branch* branch)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->branchUpstreamChanged(repo, branch);
        }
    }

    void Events::namespaceCreated(Repo* repo, Namespace* nameSpace)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->namespaceCreated(repo, nameSpace);
        }
    }

    void Events::namespaceAboutToBeDeleted(Repo* repo, Namespace* nameSpace)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->namespaceAboutToBeDeleted(repo, nameSpace);
        }
    }

    void Events::refLogChanged(Repo* repo, RefLog* reflog)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refLogChanged(repo, reflog);
        }
    }

    void Events::refLogNewEntry(Repo* repo, RefLog* reflog)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->refLogNewEntry(repo, reflog);
        }
    }

    void Events::stageCreated(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->stageCreated(repo, ref);
        }
    }

    void Events::stageAboutToBeDeleted(Repo* repo, Ref* ref)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->stageAboutToBeDeleted(repo, ref);
        }
    }

    void Events::remoteCreated(Repo* repo, Remote* remote)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->remoteCreated(repo, remote);
        }
    }

    void Events::remoteAboutToBeDeleted(Repo* repo, Remote* remote)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->remoteAboutToBeDeleted(repo, remote);
        }
    }

    void Events::remoteModified(Repo* repo, Remote* remote)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->remoteModified(repo, remote);
        }
    }

    void Events::submoduleCreated(Repo* repo, Submodule* submodule)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->submoduleCreated(repo, submodule);
        }
    }

    void Events::submoduleAboutToBeDeleted(Repo* repo, Submodule* submodule)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->submoduleAboutToBeDeleted(repo, submodule);
        }
    }

    void Events::submoduleMoved(Repo* repo, Submodule* submodule)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->submoduleMoved(repo, submodule);
        }
    }

    void Events::repositoryStateChanged(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryStateChanged(repo);
        }
    }

    void Events::indexUpdated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->indexUpdated(repo);
        }
    }

    void Events::workTreeUpdated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->workTreeUpdated(repo);
        }
    }


}
