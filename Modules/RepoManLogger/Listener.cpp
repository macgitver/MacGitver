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

#include "libRepoMan/RepoMan.hpp"
#include "libRepoMan/Frontend/Tag.hpp"
#include "libRepoMan/Frontend/Branch.hpp"
#include "libRepoMan/Frontend/Submodule.hpp"

#include "Listener.hpp"
#include "TemplateNames.hpp"

Listener::Listener(Log::Channel channel)
    : repoManChannel(channel)
{
    RM::Events::addReceiver(this);
}

Listener::~Listener()
{
    RM::Events::delReceiver(this);
}

void Listener::repositoryOpened(const RM::Frontend::Repo& repo)
{
    Log::Event e = Log::Event::create(TMPL_REPO_ACTIVITY);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("Action"),     tr("opened"));
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::repositoryAboutToClose(const RM::Frontend::Repo& repo)
{
    Log::Event e = Log::Event::create(TMPL_REPO_ACTIVITY);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("Action"),     tr("closed"));
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::repositoryActivated(const RM::Frontend::Repo& repo)
{
    Log::Event e = Log::Event::create(TMPL_REPO_ACTIVITY);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("Action"),     tr("activated"));
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::repositoryDeactivated(const RM::Frontend::Repo& repo)
{
    // We don't want to report deactivation
}

void Listener::objectCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object)
{
}

void Listener::objectAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Base& object)
{
}

void Listener::refTreeNodeCreated(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node)
{
}

void Listener::refTreeNodeAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& node)
{
}

void Listener::refCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::refAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::refMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::refHeadDetached(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::tagCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag)
{
    Log::Event e = Log::Event::create(TMPL_FOUND_NEW_REF);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("Type"),       tr("tag"));
    e.setParam(QStringLiteral("ObjName"),    tag.displayName());
    e.setParam(QStringLiteral("SHA"),        tag.displaySha1());
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::tagAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Tag& tag)
{
}

void Listener::branchCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch)
{
}

void Listener::branchAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch)
{
}

void Listener::branchMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch)
{
    Log::Event e = Log::Event::create(TMPL_BRANCH_MOVED);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("ObjName"),    branch.displayName());
    e.setParam(QStringLiteral("SHA"),        branch.displaySha1());
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::branchUpstreamChanged(const RM::Frontend::Repo& repo, const RM::Frontend::Branch& branch)
{
}

void Listener::namespaceCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace)
{
}

void Listener::namespaceAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Namespace& nameSpace)
{
}

void Listener::refLogChanged(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog)
{
}

void Listener::refLogNewEntry(const RM::Frontend::Repo& repo, const RM::Frontend::RefLog& reflog)
{
}

void Listener::stageCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::stageAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref)
{
}

void Listener::remoteCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote)
{
}

void Listener::remoteAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote)
{
}

void Listener::remoteModified(const RM::Frontend::Repo& repo, const RM::Frontend::Remote& remote)
{
}

void Listener::submoduleCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule)
{
    Log::Event e = Log::Event::create(TMPL_FOUND_NEW_SM);
    Q_ASSERT(e);

    e.setParam(QStringLiteral("ObjName"),    submodule.displayName());
    e.setParam(QStringLiteral("RepoName"),   repo.displayAlias());

    repoManChannel.addEvent(e);
}

void Listener::submoduleAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule)
{
}

void Listener::submoduleMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Submodule& submodule)
{
}

void Listener::repositoryStateChanged(const RM::Frontend::Repo& repo)
{
}

void Listener::indexUpdated(const RM::Frontend::Repo& repo)
{
}

void Listener::workTreeUpdated(const RM::Frontend::Repo& repo)
{
}
