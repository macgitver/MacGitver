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
#include "WorkingTreeModel.h"
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
    if (!i.isValid()) {
        return false;
    }

    WorkingTreeModel* model = qobject_cast<WorkingTreeModel*>(sourceModel());
    if (!model) {
        Q_ASSERT(false);
        return true;
    }

    WorkingTreeAbstractItem* it = model->indexToItem(i);
    if (!it->isDirectory()) {

        // If filter says "show it" and it's not a directory, then accpet it
        QVariant v = i.data(WorkingTreeAbstractItem::StatusRole);
        if (!v.isValid()) {
            return true;
        }

        Git::StatusFlags f = Git::StatusFlags( v.toInt() );
        return (f & mStatusFilter);
    }

    // If it is a directory, check if any child is accepted:
    int numKids = model->rowCount(i);
    if (!numKids) {
        // If we are a directory and got no kids, don't accept it
        return false;
    }

    bool anyChildVisible = false;
    for (int cnt = 0; cnt < numKids; cnt++) {
        anyChildVisible |= filterAcceptsRow(cnt, i);
    }

    // We are a directory, then we accept if we have any accepted child
    return anyChildVisible;
}
