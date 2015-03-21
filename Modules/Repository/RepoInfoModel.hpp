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

#ifndef MGV_REPO_INFO_MODEL_HPP
#define MGV_REPO_INFO_MODEL_HPP

#include <QAbstractItemModel>

namespace RM
{
    class Repo;
    class RepoMan;
}

class RepoInfoModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    RepoInfoModel();

    enum ExtraRoles
    {
        IsActive = Qt::UserRole
    };

public:
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;

public:
    RM::Repo* index2Info( const QModelIndex& index ) const;
    QModelIndex info2Index( RM::Repo* info ) const;

public slots:
    void invalidateRepository(RM::Repo* info);
    void repositoryOpened(RM::Repo* info);
    void repositoryChildAdded(RM::Repo* parent, RM::Repo* child);

private:
    RM::RepoMan*    mRepoMan;
};

#endif
