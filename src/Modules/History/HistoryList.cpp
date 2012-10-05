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

#include "Widgets/HeaderView.h"

#include "HistoryList.h"
#include "HistoryModel.h"
#include "HistoryEntry.h"

HistoryList::HistoryList()
    : ConfigUser( "History" )
{
    setRootIsDecorated( false );
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
    QTreeView::setModel( model );

    connect( selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this, SLOT(onCurrentChanged()) );

    HeaderView* hv = new HeaderView( Qt::Horizontal );
    setHeader( hv );
    hv->setConfigName( configSubPath( "Columns" ) );
}

void HistoryList::configChanged( const QString& subPath, const QVariant& value )
{
    qDebug() << "ConfigChange:" << subPath << "=>" << value;
}
