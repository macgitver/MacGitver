/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef MGV_BRANCHES_MODEL_HPP
#define MGV_BRANCHES_MODEL_HPP

#include <QAbstractItemModel>

#include "libGitWrap/Repository.hpp"

class BranchesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BranchesModel( QObject* parent );
    ~BranchesModel();

public:
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QModelIndex index( int row, int column = 0, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    bool hasChildren( const QModelIndex& parent ) const;

public:
    void setRepository( Git::Repository repo );
    void rereadBranches();

private:
    class Item;
    class Scope;
    class NameSpace;
    class Branch;
    class Remote;
    Git::Repository mRepo;
    Item* mRoot;
};

#endif
