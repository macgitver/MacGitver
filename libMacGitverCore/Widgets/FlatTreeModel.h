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

#ifndef MGV_FLAT_TREE_MODEL_H
#define MGV_FLAT_TREE_MODEL_H

#include "libMacGitverCore/MacGitverApi.hpp"

#include <QAbstractListModel>

class FlatTreeModelPrivate;

class MGV_CORE_API FlatTreeModel : public QAbstractListModel
{
    friend class FlatTreeModelPrivate;
    Q_OBJECT
public:
    explicit FlatTreeModel( QObject* parent = 0 );
    FlatTreeModel( const QStringList& entries, QObject* parent = 0 );
    FlatTreeModel( const QChar separator, const QStringList& entries, QObject* parent = 0 );
    ~FlatTreeModel();

public:
    void clear();

    void add( const QString& entry );
    void add( const QStringList& entries );

    void remove( const QString& entry );
    void remove( const QStringList& entries );

    QIcon defaultHeaderIcon() const;
    QIcon defaultDataIcon() const;

    void setDefaultHeaderIcon( const QIcon& icon );
    void setDefaultDataIcon( const QIcon& icon );

public:
    int rowCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    Qt::ItemFlags flags( const QModelIndex& index ) const;

private:
    FlatTreeModelPrivate* d;
};

#endif
