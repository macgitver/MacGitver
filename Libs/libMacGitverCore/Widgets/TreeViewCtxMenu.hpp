/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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

#ifndef MGV_CORE_TREE_VIEW_CTXMENU_HPP
#define MGV_CORE_TREE_VIEW_CTXMENU_HPP

#include <QTreeView>

#include "libMacGitverCore/MacGitverApi.hpp"

class MGV_CORE_API TreeViewCtxMenu : public QTreeView
{
    Q_OBJECT
public:
    TreeViewCtxMenu( QWidget* parent = NULL );

protected:
    void contextMenuEvent( QContextMenuEvent* ev );

signals:
    void contextMenu( const QModelIndex& index, const QPoint& globalPos );
};

#endif
