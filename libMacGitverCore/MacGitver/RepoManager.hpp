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

#ifndef MGV_REPO_MANAGER_HPP
#define MGV_REPO_MANAGER_HPP

#include <QObject>

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/MacGitver/RepositoryInfo.hpp"

class MGV_CORE_API RepoManager : public QObject
{
    Q_OBJECT
public:
    RepoManager();
    ~RepoManager();

public:
    void open();
    bool open( const QString& path );
    bool open( const Git::Repository& repo );

    void closeAll();

    RepositoryInfo* activeRepository();
    void activate( RepositoryInfo* repository );

    RepositoryInfo::List repositories() const;

    RepositoryInfo* repoByPath( const QString& basePath, bool searchSubmodules );

    void internalClosedRepo(RepositoryInfo* repository);
    void internalActivate( RepositoryInfo* repository );

signals:
    void firstRepositoryOpened();
    void lastRepositoryClosed();
    void repositoryOpened( RepositoryInfo* repo );
    void repositoryClosed();
    void repositoryActivated( RepositoryInfo* repo );

private slots:
    void onRepositoryOpenHelper();

private:
    RepositoryInfo::List    mRepos;
    RepositoryInfo*         mActiveRepo;
};

#endif
