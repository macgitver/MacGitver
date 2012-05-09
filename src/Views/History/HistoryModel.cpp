
#include "Views/History/HistoryModel.h"
#include "Views/History/HistoryEntry.h"

HistoryModel::HistoryModel( QObject* parent )
	: QAbstractTableModel( parent )
	, mEntries( NULL )
{
}

int HistoryModel::rowCount( const QModelIndex& parent ) const
{
	return mEntries ? mEntries->count() : 0;
}

int HistoryModel::columnCount( const QModelIndex& parent ) const
{
	return 7;
}

QVariant HistoryModel::data( const QModelIndex& index, int role ) const
{
	if( !index.isValid() )
		return QVariant();

	HistoryEntry* e = mEntries->at( index.row() );
	Q_ASSERT( e );

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

HistoryEntries* HistoryModel::entries()
{
	if( !mEntries )
	{
		mEntries = new HistoryEntries();

		connect( mEntries, SIGNAL(beforeAppend()),
				 this, SLOT(beforeAppend()) );

		connect( mEntries, SIGNAL(afterAppend()),
				 this, SLOT(afterAppend()) );

		connect( mEntries, SIGNAL(beforeClear()),
				 this, SLOT(beforeClear()) );

		connect( mEntries, SIGNAL(afterClear()),
				 this, SLOT(afterClear()) );
	}

	return mEntries;
}

void HistoryModel::beforeClear()
{
	beginResetModel();
}

void HistoryModel::afterClear()
{
	endResetModel();
}

void HistoryModel::beforeAppend()
{
	beginInsertRows( QModelIndex(), mEntries->count(), mEntries->count() );
}

void HistoryModel::afterAppend()
{
	endInsertRows();
}
