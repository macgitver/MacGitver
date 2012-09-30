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
	mMode = modeSimple;
}

HistoryModel::~HistoryModel()
{
	qDeleteAll( mEntries );
}

void HistoryModel::setRepository( Git::Repository repo )
{
	mRepo = repo;

	if( !mRepo.isValid() )
	{
		beginResetModel();

		qDeleteAll( mEntries );
		mEntries.clear();

		endResetModel();
	}
}

int HistoryModel::columnMap( int index ) const
{
	switch( mMode )
	{
	case modeSimple:
		switch( index )
		{
		case -1:	return 4;
		case 0:		return colGraph;
		case 1:		return colMessage;
		case 2:		return colAuthor;
		case 3:		return colAuthorDate;
		default:	return -1;
		}

	case modeFull:
		switch( index )
		{
		case -1:	return 7;
		case 0:		return colGraph;
		case 1:		return colMessage;
		case 2:		return colAuthor;
		case 3:		return colAuthorDate;
		case 4:		return colCommitter;
		case 5:		return colCommitterDate;
		case 6:		return colSHA1;
		default:	return -1;
		}

	case modeFancy:
		return 1;
	}

	return -1;
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
	return columnMap( -1 );
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
		switch( columnMap( index.column() ) )
		{
		case colGraph:			return QVariant();
		case colMessage:		return e->message();
		case colAuthor:			return e->author().fullName();
		case colAuthorDate:		return e->author().when();
		case colCommitter:		return e->committer().fullName();
		case colCommitterDate:	return e->committer().when();
		case colSHA1:			return e->id().toString();
		default:				return QVariant();
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
	case colGraph:			return trUtf8( "Graph" );
	case colMessage:		return trUtf8( "Message" );
	case colAuthor:			return trUtf8( "Author" );
	case colAuthorDate:		return trUtf8( "Author date" );
	case colCommitter:		return trUtf8( "Comitter" );
	case colCommitterDate:	return trUtf8( "Committer date" );
	case colSHA1:			return trUtf8( "SHA1" );
	default:				return QVariant();
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

	Git::Result r;
	Git::ObjectCommit commit = mRepo.lookupCommit( e->id(), r );
	if( !r || !commit.isValid() )
	{
		e->populate( commit );

		emit dataChanged( index( row, 0 ), index( row, columnMap( -1 ) - 1 ) );
	}
	else
	{
		qDebug() << "Failed to lookup a commit" << r.errorText();
	}
}

void HistoryModel::append( HistoryEntry* entry )
{
	mEntries.append( entry );
}
