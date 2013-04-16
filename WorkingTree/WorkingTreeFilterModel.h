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

#ifndef MGV_MODULE_WORKINGTREEFILTERMODEL_H
#define MGV_MODULE_WORKINGTREEFILTERMODEL_H

#include "libGitWrap/GitWrap.hpp"

#include <QSortFilterProxyModel>

class WorkingTreeFilterModel : public QSortFilterProxyModel
{
public:
    WorkingTreeFilterModel(QObject *parent = 0);
    ~WorkingTreeFilterModel();

    Git::StatusFlags filter() const;
    void setFilter(Git::StatusFlags value);

private:
    bool filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    Git::StatusFlags    mStatusFilter;
};


#define ALL_FILE_STATUS_FILTERS Git::StatusFlags( \
    Git::FileIgnored | \
    Git::FileWorkingTreeDeleted | \
    Git::FileWorkingTreeModified | \
    Git::FileUnchanged | \
    Git::FileWorkingTreeNew )

#endif // MGV_MODULE_WORKINGTREEFILTERMODEL_H
