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
    mStatusFilter = WTF_All;
}

WorkingTreeFilterModel::~WorkingTreeFilterModel()
{
}

WorkingTreeFilters WorkingTreeFilterModel::filter() const
{
    return mStatusFilter;
}

void WorkingTreeFilterModel::setFilter(WorkingTreeFilters value)
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
    WorkingTreeFilters f = WorkingTreeFilters( v.toInt() );
    return ( f & mStatusFilter );
}
