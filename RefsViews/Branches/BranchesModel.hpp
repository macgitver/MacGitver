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

#include "Branches/BranchesViewData.hpp"

class BranchesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    class Item
    {
    public:
        Item()
            : parent( NULL )
            , text()
        {}

        Item( Item* p, const QString& t )
            : parent( p )
            , text( t )
        {
            Q_ASSERT( p );
            p->children.append( this );
        }

        virtual ~Item()
        {
            if( parent )
            {
                parent->children.removeOne( this );
            }
            qDeleteAll( children );
        }

    public:
        Item* parent;
        QList< Item* > children;
        QString text;

        virtual QVariant data( int col, int role ) const { return QVariant(); }
    };

public:
    BranchesModel( BranchesViewData* parent );
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
    void rereadBranches();

private:
    class Scope;
    class NameSpace;
    class Branch;
    class Remote;

    BranchesViewData*   mData;
    Item*               mRoot;
};

#endif
