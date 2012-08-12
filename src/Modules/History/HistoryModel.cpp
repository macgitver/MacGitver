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

#include "HistoryModel.h"
#include "HistoryEntry.h"

HistoryModel::HistoryModel( QObject* parent )
	: QAbstractTableModel( parent )
{
}

HistoryModel::~HistoryModel()
{
	qDeleteAll( mEntries );
}

void HistoryModel::setRepository( Git::Repository repo )
{
	mRepo = repo;
}

int HistoryModel::rowCount( const QModelIndex& parent ) const
{
	if( parent.isValid() )
	{
		return 0;
	}
	return mEntries.count();
}

int HistoryModel::columnCount( const QModelIndex& parent ) const
{
	if( parent.isValid() )
	{
		return 0;
	}
	return 7;
}

HistoryEntry* HistoryModel::at( int row, bool populate ) const
{
	HistoryEntry* e = mEntries[ row ];
	Q_ASSERT( e );

	if( populate && !e->isPopulated() )
	{
		QMetaObject::invokeMethod( (HistoryModel*)this, "ensurePopulated", Qt::QueuedConnection,
								   Q_ARG( int, row ) );
		return NULL;
	}
	return e;
}

HistoryEntry* HistoryModel::indexToEntry( const QModelIndex& index ) const
{
	if( !index.isValid() )
	{
		return NULL;
	}

	return at( index.row() );
}

QVariant HistoryModel::data( const QModelIndex& index, int role ) const
{
	HistoryEntry* e = indexToEntry( index );
	if( !e )
	{
		return QVariant();
	}

	switch( role )
	{
	case Qt::DisplayRole:
		switch( index.column() )
		{
		case 0:		return QVariant();
		case 1:		return e->message();
		case 2:		return e->author().fullName();
		case 3:		return e->author().when();
		case 4:		return e->committer().fullName();
		case 5:		return e->committer().when();
		case 6:		return e->id().toString();
		default:	return QVariant();
		}

	default:
		return QVariant();
	}
}

QVariant HistoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Vertical )
		return QVariant();

	if( role != Qt::DisplayRole )
		return QVariant();

	switch( section )
	{
	case 0:		return trUtf8( "Graph" );
	case 1:		return trUtf8( "Message" );
	case 2:		return trUtf8( "Author" );
	case 3:		return trUtf8( "Author date" );
	case 4:		return trUtf8( "Comitter" );
	case 5:		return trUtf8( "Committer date" );
	case 6:		return trUtf8( "SHA1" );
	default:	return QVariant();
	}
}

void HistoryModel::beforeClear()
{
	beginResetModel();
}

void HistoryModel::afterClear()
{
	endResetModel();
}

void HistoryModel::updateRow( int row )
{
	dataChanged( index( row, 0 ), index( row, columnCount() ) );
}

void HistoryModel::beforeAppend()
{
	beginInsertRows( QModelIndex(), mEntries.count(), mEntries.count() );
}

void HistoryModel::afterAppend()
{
	endInsertRows();
}

void HistoryModel::ensurePopulated( int row )
{
	Q_ASSERT( row < mEntries.count() );

	HistoryEntry* e = mEntries[ row ];
	if( !e || e->isPopulated() )
	{
		return;
	}

	Git::ObjectCommit commit = mRepo.lookupCommit( e->id() );
	e->populate( commit );

	emit dataChanged( index( row, 0 ), index( row, 6 ) );
}

void HistoryModel::append( HistoryEntry* entry )
{
	mEntries.append( entry );
}
