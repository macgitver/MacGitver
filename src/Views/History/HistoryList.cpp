
#include "Views/History/HistoryList.h"

HistoryModel::HistoryModel( QObject* parent )
	: QAbstractTableModel( parent )
{
}

int HistoryModel::rowCount( const QModelIndex& parent ) const
{
	return mHistoryEntries.count();
}

int HistoryModel::columnCount( const QModelIndex& parent ) const
{
	return 5;
}

QVariant HistoryModel::data( const QModelIndex& index, int role ) const
{
	return QVariant();
}

HistoryList::HistoryList()
{
	setRootIsDecorated( false );
}
