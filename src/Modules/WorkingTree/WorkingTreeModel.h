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

#ifndef MGV_WORKING_TREE_MODEL_H
#define MGV_WORKING_TREE_MODEL_H

#include <QAbstractItemModel>

#include "libGitWrap/Repository.h"

#include "WorkingTreeFilters.h"

class WorkingTreeAbstractItem;
class WorkingTreeDirItem;
class WorkingTreeFileItem;

class WorkingTreeModel : public QAbstractItemModel
{
    friend class WorkingTreeAbstractItem;
    friend class WorkingTreeDirItem;
    friend class WorkingTreeFileItem;
    Q_OBJECT
public:
    WorkingTreeModel( Git::Repository repo, QObject* parent = 0 );
    ~WorkingTreeModel();

public:
    void setRepository( Git::Repository repo );
    void setFilters( WorkingTreeFilters filters );

public:
    QVariant data( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column,
                       const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;

public:
    void update();

private:
    Git::Repository     mRepo;
    WorkingTreeDirItem* mRootItem;
    WorkingTreeFilters  mFilters;
};

#endif
