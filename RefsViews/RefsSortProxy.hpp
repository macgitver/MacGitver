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

#ifndef REFS_SORT_PROXY_HPP
#define REFS_SORT_PROXY_HPP

#include "libGitWrap/GitWrap.hpp"

#include <QSortFilterProxyModel>

class RefsSortProxy : public QSortFilterProxyModel
{
public:
    RefsSortProxy(QObject *parent = 0);
    ~RefsSortProxy();

public:
    QModelIndex deeplyMapToSource( QModelIndex current ) const;

private:
    bool filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

};

#endif // REFS_SORT_PROXY_HPP
