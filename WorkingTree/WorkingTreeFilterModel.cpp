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

#include "WorkingTreeFilterModel.h"

#include "WorkingTreeAbstractItem.h"


WorkingTreeFilterModel::WorkingTreeFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    mStatusFilter = ALL_FILE_STATUS_FILTERS;
}

WorkingTreeFilterModel::~WorkingTreeFilterModel()
{
}

Git::StatusFlags WorkingTreeFilterModel::filter() const
{
    return mStatusFilter;
}

void WorkingTreeFilterModel::setFilter(Git::StatusFlags value)
{
    mStatusFilter = value;
    invalidateFilter();
}

bool WorkingTreeFilterModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    Q_UNUSED( sourceColumn )
    Q_UNUSED( sourceParent )

    return true;
}

bool WorkingTreeFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex i = sourceModel()->index( sourceRow, 0, sourceParent );
    if ( !i.isValid() )
        return false;

    QVariant v = i.data( WorkingTreeAbstractItem::StatusRole );
    if ( !v.isValid() )
        return true;

    Git::StatusFlags f = Git::StatusFlags( v.toInt() );
    return ( f & mStatusFilter );
}
