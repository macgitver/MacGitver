/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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
    RepoManager( QObject* parent );
    ~RepoManager();

public:
    void open();
    void open( const QString& path );
    void closeAll();

    RepositoryInfo* activeRepository();
    void activate( RepositoryInfo* repository );

    RepositoryInfo::List repositories() const;

    RepositoryInfo* repoByPath( const QString& basePath, bool searchSubmodules );

    void internalActivate( RepositoryInfo* repository );

signals:
    void repositoryOpened( RepositoryInfo* repo );
    void repositoryClosed( RepositoryInfo* repo );
    void repositoryActivated( RepositoryInfo* repo);

private:
    void open( const Git::Repository& repo );

private slots:
    void onRepositoryOpenHelper();

private:
    RepositoryInfo::List    mRepos;
    RepositoryInfo*         mActiveRepo;
};

#endif
