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

#ifndef HISTORY_LIST_H
#define HISTORY_LIST_H

#include "libMacGitverCore/Config/ConfigUser.h"

#include "libGitWrap/ObjectId.hpp"
#include "libMacGitverCore/Widgets/TreeViewCtxMenu.hpp"

#include "hic_HistoryCtxMenu.h"

class HistoryModel;

class HistoryList : public TreeViewCtxMenu, private HistoryTreeCtxMenu, private ConfigUser
{
    Q_OBJECT
public:
    HistoryList( QWidget* parent = 0 );

public:
    void setModel( QAbstractItemModel* model );

signals:
    void currentCommitChanged( const Git::ObjectId& sha1 );

private:
    void configChanged( const QString& subPath, const QVariant& value );

    QModelIndex deeplyMapToSource( QModelIndex current ) const;

private slots:
    void onCurrentChanged();
    void contextMenu(const QModelIndex& index, const QPoint& globalPos);

    void onCheckout();
    void onCreateBranch();

private:
    HistoryModel *      mModel;

private:
    inline void checkoutBranch(Git::Reference branch);
};

#endif
