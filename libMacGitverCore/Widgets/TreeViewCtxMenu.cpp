/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include <QContextMenuEvent>

#include "TreeViewCtxMenu.hpp"

TreeViewCtxMenu::TreeViewCtxMenu( QWidget* parent )
    : QTreeView( parent )
{
}

void TreeViewCtxMenu::contextMenuEvent( QContextMenuEvent* ev )
{
    if( ev->reason() == QContextMenuEvent::Keyboard )
    {
        emit contextMenu( QModelIndex(), ev->globalPos() );
        ev->accept();
    }
    else if( ev->reason() == QContextMenuEvent::Mouse )
    {
        QModelIndex idx = indexAt( ev->pos() );
        emit contextMenu( idx, ev->globalPos() );
        ev->accept();
    }
}
