/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_FSWATCHER_H
#define MGV_FSWATCHER_H

#include <QObject>

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/MacGitverApi.hpp"

class QFileSystemWatcher;

class FSWatcherPrivate;

class MGV_CORE_API FSWatcher : public QObject
{
    Q_OBJECT
private:
    friend class FSWatcherPrivate;

public:
    FSWatcher( QObject* parent );
    ~FSWatcher();

public:
    void setRepository( Git::Repository repo );

signals:
    void configChanged();
    void refsChanged();
    void refLogChanged();
    void headChanged();
    void repoGitFileChanged();
    void workingTreeChanged();
    void descriptionChanged();
    void indexChanged();
    void modeChanged();

private slots:
    void directoryChanged( const QString& path );
    void spitOutChanges();

private:
    FSWatcherPrivate* d;
};

#endif
