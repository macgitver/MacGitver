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

#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Widgets/HeaderView.h"

#include "IndexTreeItemView.h"
#include "WorkingTreeAbstractItem.h"

#include <QAbstractProxyModel>

IndexTreeItemView::IndexTreeItemView()
{
#ifdef Q_OS_MACX
    setAttribute( Qt::WA_MacShowFocusRect, false );
#endif

    mHeader = new HeaderView( Qt::Horizontal );
    setHeader( mHeader );
    mHeader->setConfigName( QLatin1String( "Indextree/Columns" ) );

    StageViewCtxMenu::setupActions( this );

    connect( this, SIGNAL(contextMenu(QModelIndex, QPoint)),
             this, SLOT(contextMenu(QModelIndex, QPoint)) );
}

void IndexTreeItemView::onWtCtxUnstage()
{
    // TODO: unstage file
}

void IndexTreeItemView::contextMenu( const QModelIndex& index, const QPoint& globalPos )
{
    if ( !index.isValid() )
        return;

    WorkingTreeAbstractItem* item =
            static_cast<WorkingTreeAbstractItem*>( index.internalPointer() );

    if( item ) // && !item->isDirectory() )
    {
        Heaven::Menu* menu = menuCtxMenuStageView;
        //menu->setActivationContext( item );

        menu->showPopup( globalPos );
}

QModelIndex IndexTreeItemView::deeplyMapToSource( QModelIndex current ) const
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
