
#ifndef HISTORY_MODEL_H
#define HISTORY_MODEL_H

#include <QAbstractTableModel>

class HistoryEntries;

class HistoryModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	HistoryModel( QObject* parent = NULL );

public:
	int rowCount( const QModelIndex& parent ) const;
	int columnCount( const QModelIndex& parent ) const;
	QVariant data( const QModelIndex& index, int role ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

public:
	HistoryEntries* entries();

private slots:
	void beforeClear();
	void afterClear();
	void beforeAppend();
	void afterAppend();

private:
	HistoryEntries* mEntries;
};

#endif
