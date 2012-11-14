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

#include "libGitWrap/Repository.h"

#include "RepositoryInfo.hpp"

class RepoManager : public QObject
{
    Q_OBJECT
public:
    RepoManager();

signals:
    void repositoryOpened( RepositoryInfo* repo );
    void repositoryClosed( RepositoryInfo* repo );
    void repositoryActivated( RepositoryInfo* repo);

private:
    RepositoryInfo::List    mRepos;
};

#endif
