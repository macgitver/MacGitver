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

#include <QObject>

class QTimer;

#include "libMacGitverCore/Config/ConfigUser.h"

namespace RM
{

    class AutoRefresher : public QObject
    {
        Q_OBJECT
    public:
        AutoRefresher(QObject* parent);
        ~AutoRefresher();

    public:
        bool refreshEnabled() const;
        bool refreshGitEnabled() const;
        bool refreshIndexEnabled() const;
        bool refreshWorktreeEnabled() const;

        int gitRefreshInterval() const;
        int indexRefreshInterval() const;
        int worktreeRefreshInterval() const;

        void setRefreshEnabled(bool enabled);
        void setRefreshGitInterval(int interval, bool force = false);
        void setRefreshIndexInterval(int interval, bool force = false);
        void setRefreshWorktreeInterval(int interval, bool force = false);

    private slots:
        void onRefreshGit();
        void onRefreshIndex();
        void onRefreshWorktree();

    private:
        void forcedUpdate();

    private:
        QTimer*     refreshGitTimer;
        QTimer*     refreshIndexTimer;
        QTimer*     refreshWorktreeTimer;
    };

}
