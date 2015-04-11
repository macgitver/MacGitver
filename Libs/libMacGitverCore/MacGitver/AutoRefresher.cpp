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

#include "libMacGitverCore/MacGitver/AutoRefresher.hpp"
#include "AutoRefresherCfg.hpp"

#include "libRepoMan/RepoMan.hpp"

#include <QTimer>
#include <QDebug>

AutoRefresher::AutoRefresher()
    : QObject()
    , refreshGitTimer(nullptr)
    , refreshIndexTimer(nullptr)
    , refreshWorktreeTimer(nullptr)
{
    new AutoRefresherCfg(this);

    forcedUpdate();
}

AutoRefresher::~AutoRefresher()
{
}

bool AutoRefresher::refreshEnabled() const
{
    return AutoRefresherCfg::refreshEnabled();
}

bool AutoRefresher::refreshGitEnabled() const
{
    return AutoRefresherCfg::gitInterval() != 0;
}

bool AutoRefresher::refreshIndexEnabled() const
{
    return AutoRefresherCfg::indexInterval() != 0;
}

bool AutoRefresher::refreshWorktreeEnabled() const
{
    return AutoRefresherCfg::worktreeInterval() != 0;
}

void AutoRefresher::setRefreshEnabled(bool enabled)
{
    AutoRefresherCfg::setRefreshEnabled(enabled);
    forcedUpdate();
}

void AutoRefresher::forcedUpdate()
{
    setRefreshGitInterval(AutoRefresherCfg::gitInterval(), true);
    setRefreshIndexInterval(AutoRefresherCfg::indexInterval(), true);
    setRefreshWorktreeInterval(AutoRefresherCfg::worktreeInterval(), true);
}

void AutoRefresher::setRefreshGitInterval(int interval, bool force)
{
    if (interval < 15) {
        interval = 0;
    }

    if (interval == AutoRefresherCfg::gitInterval() ) {
        if (!force) {
            return;
        }
    }
    else {
        AutoRefresherCfg::setGitInterval(interval);
    }

    if (interval) {
        if (!refreshGitTimer) {
            refreshGitTimer = new QTimer(this);
            connect(refreshGitTimer, SIGNAL(timeout()),
                    this, SLOT(onRefreshGit()));
        }
        refreshGitTimer->setInterval(interval * 1000);
        refreshGitTimer->start();
    }
    else {
        if (refreshGitTimer) {
            refreshGitTimer->stop();
            refreshGitTimer->deleteLater();
            refreshGitTimer = NULL;
        }
    }
}

void AutoRefresher::setRefreshIndexInterval(int interval, bool force)
{
    if (interval < 30) {
        interval = 0;
    }

    if (interval == AutoRefresherCfg::indexInterval() ) {
        if (!force) {
            return;
        }
    }
    else {
        AutoRefresherCfg::setIndexInterval(interval);
    }

    if (interval) {
        if (!refreshIndexTimer) {
            refreshIndexTimer = new QTimer(this);
            connect(refreshIndexTimer, SIGNAL(timeout()),
                    this, SLOT(onRefreshIndex()));
        }
        refreshIndexTimer->setInterval(interval * 1000);
        refreshIndexTimer->start();
    }
    else {
        if (refreshIndexTimer) {
            refreshIndexTimer->stop();
            refreshIndexTimer->deleteLater();
            refreshIndexTimer = NULL;
        }
    }
}

void AutoRefresher::setRefreshWorktreeInterval(int interval, bool force)
{
    if (interval < 120) {
        interval = 0;
    }

    if (interval == AutoRefresherCfg::worktreeInterval() ) {
        if (!force) {
            return;
        }
    }
    else {
        AutoRefresherCfg::setWorktreeInterval(interval);
    }

    if (interval) {
        if (!refreshWorktreeTimer) {
            refreshWorktreeTimer = new QTimer(this);
            connect(refreshWorktreeTimer, SIGNAL(timeout()),
                    this, SLOT(onRefreshWorktree()));
        }
        refreshWorktreeTimer->setInterval(interval * 1000);
        refreshWorktreeTimer->start();
    }
    else {
        if (refreshWorktreeTimer) {
            refreshWorktreeTimer->stop();
            refreshWorktreeTimer->deleteLater();
            refreshWorktreeTimer = NULL;
        }
    }
}

int AutoRefresher::gitRefreshInterval() const
{
    return AutoRefresherCfg::gitInterval();
}

int AutoRefresher::indexRefreshInterval() const
{
    return AutoRefresherCfg::indexInterval();
}

int AutoRefresher::worktreeRefreshInterval() const
{
    return AutoRefresherCfg::worktreeInterval();
}

void AutoRefresher::onRefreshGit()
{
    qDebug() << "Refresh Repository currently not implemented.";
}

void AutoRefresher::onRefreshIndex()
{
    qDebug() << "Refresh Index currently not implemented.";
}

void AutoRefresher::onRefreshWorktree()
{
    qDebug() << "Refresh Worktree currently not implemented.";
}
