/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#include "RefsSortProxy.hpp"


RefsSortProxy::RefsSortProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

RefsSortProxy::~RefsSortProxy()
{
}

QModelIndex RefsSortProxy::deeplyMapToSource(QModelIndex current) const
{
    while( current.isValid() )
    {
        const QAbstractProxyModel* apm = qobject_cast< const QAbstractProxyModel* >( current.model() );
        if( !apm )
            return current;

        current = apm->mapToSource( current );
    }

    return QModelIndex();
}

bool RefsSortProxy::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    Q_UNUSED( sourceColumn )
    Q_UNUSED( sourceParent )

    return true;
}

bool RefsSortProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex i = sourceModel()->index( sourceRow, 0, sourceParent );
    if ( !i.isValid() )
        return false;

    QString itemName = i.data().toString();
    bool display = itemName.right(4) != QString::fromUtf8( "HEAD" );

    return display;
}
