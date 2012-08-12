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

#ifndef HISTORY_MODEL_H
#define HISTORY_MODEL_H

#include <QAbstractTableModel>

#include "GitWrap/Repository.h"

class HistoryEntry;
class HistoryEntries;

class HistoryModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	HistoryModel( QObject* parent = NULL );

public:
	HistoryEntry* indexToEntry( const QModelIndex& index ) const;
	int rowCount( const QModelIndex& parent ) const;
	int columnCount( const QModelIndex& parent ) const;
	QVariant data( const QModelIndex& index, int role ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

public:
	void setRepository( Git::Repository repo );

public slots:
	void ensurePopulated( int row );

public:
	HistoryEntries* entries();

private slots:
	void beforeClear();
	void afterClear();
	void beforeAppend();
	void afterAppend();

private:
	Git::Repository	mRepo;
	HistoryEntries* mEntries;
};

#endif
