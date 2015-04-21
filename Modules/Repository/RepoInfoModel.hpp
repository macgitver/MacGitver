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

#pragma once

#include <QAbstractItemModel>

#include "libMacGitverCore/RepoMan/Frontend/Repo.hpp"

class RepoInfoModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    RepoInfoModel();
    ~RepoInfoModel();

    enum ExtraRoles
    {
        IsActive = Qt::UserRole
    };

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& child) const;

public:
    RM::Frontend::Repo index2Repo(const QModelIndex& index) const;
    QModelIndex repo2Index(const RM::Frontend::Repo& repo) const;

private:
    struct RepoInfo;
    using RepoInfos = QVector<RepoInfo*>;

private:
    RepoInfo* index2info(const QModelIndex& index) const;

private slots:
    void invalidateRepository(const RM::Frontend::Repo& repo);
    void repositoryOpened(const RM::Frontend::Repo& repo);
    void repositoryChildAdded(const RM::Frontend::Repo& parent, const RM::Frontend::Repo& child);

private:
    RepoInfos mRoots;
};
