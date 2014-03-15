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

#include <QDebug>

#include "libMacGitverCore/Widgets/HeaderView.h"

#include "HistoryList.h"
#include "HistoryModel.h"
#include "HistoryEntry.h"

HistoryList::HistoryList( QWidget* parent )
    : TreeViewCtxMenu( parent )
    , ConfigUser( "History" )
{
    setRootIsDecorated( false );

    #ifdef Q_OS_MACX
    setAttribute( Qt::WA_MacShowFocusRect, false );
    #endif

    HeaderView* hv = new HeaderView( Qt::Horizontal );
    hv->setConfigName( configSubPath( "Columns" ) );
    setHeader( hv );

    setupActions( this );

    connect( this, SIGNAL(contextMenu(QModelIndex, QPoint)),
             this, SLOT(contextMenu(QModelIndex, QPoint)) );
}

void HistoryList::onCurrentChanged()
{
    if( currentIndex().isValid() )
    {
        const HistoryModel* hm = qobject_cast< const HistoryModel* >( currentIndex().model() );
        Q_ASSERT( hm );

        HistoryEntry* e = hm->indexToEntry( currentIndex() );
        if( e )
        {
            emit currentCommitChanged( e->id() );
            return;
        }
    }

    emit currentCommitChanged( Git::ObjectId() );
}

void HistoryList::setModel( QAbstractItemModel* model )
{
    TreeViewCtxMenu::setModel( model );
    mModel = qobject_cast< HistoryModel * >( model );

    connect( selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this, SLOT(onCurrentChanged()) );

    // This call fixes the disappearing HeaderView.
    updateGeometries();
}

void HistoryList::configChanged( const QString& subPath, const QVariant& value )
{
    //qDebug() << "ConfigChange:" << subPath << "=>" << value;
}

void HistoryList::contextMenu( const QModelIndex& index, const QPoint& globalPos )
{
    Q_ASSERT( mModel );

    QModelIndex srcIndex = deeplyMapToSource( index );
    if ( !srcIndex.isValid() )
        return;

    HistoryEntry* item = mModel->indexToEntry( srcIndex );

    Heaven::Menu* menu = 0;
    if ( item )
    {
        menu = menuCtxMenuHistory;
        //menu->setActivationContext( item );
    }

    if ( menu )
        menu->showPopup( globalPos );
}
