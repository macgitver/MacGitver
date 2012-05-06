
#ifndef HISTORY_LIST_H
#define HISTORY_LIST_H

#include <QTreeView>

#include "Core/Signature.h"
#include "Core/ObjectId.h"

class HistoryEntry
{
public:

	QString				mCommitText;
	Git::ObjectId		mSha1;
	Git::Signature		mCommiter;
	Git::Signature		mAuthor;
};

class HistoryModel : public QAbstractTableModel
{
public:
	HistoryModel( QObject* parent = NULL );

public:
	int rowCount( const QModelIndex& parent ) const;
	int columnCount( const QModelIndex& parent ) const;
	QVariant data( const QModelIndex& index, int role ) const;

private:
	QList< HistoryEntry* >	mHistoryEntries;
};

class HistoryList : public QTreeView
{
	Q_OBJECT
public:
	HistoryList();
};

#endif
