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

#include <QStringList>
#include <QSet>

#include "RefsListModel.h"

RefsListModel::RefsListModel( QObject* parent )
{
}

RefsListModel::~RefsListModel()
{
}

int RefsListModel::rowCount( const QModelIndex& parent ) const
{
	return mOrderedData.count();
}

QVariant RefsListModel::data( const QModelIndex& index, int role ) const
{
	if( !index.isValid() || index.parent().isValid() )
	{
		return QVariant();
	}

	switch( role )
	{
	case Qt::DisplayRole:
		return mOrderedData.at( index.row() )->mRefName;
	}

	return QVariant();
}

void RefsListModel::updateRef( const QString& refName )
{
}

void RefsListModel::addRef( const QString& refName )
{
	RefInfo* ref = new RefInfo;
	ref->mRefName = refName;

	mData.insert( refName, ref );

	int row = mOrderedData.count() - 1;

	beginInsertRows( QModelIndex(), row, row );
	mOrderedData.insert( row, ref );
	endInsertRows();
}

void RefsListModel::removeRef( const QString& refName )
{
}

void RefsListModel::syncTo( const QStringList& refs )
{
	QSet< QString > oldRefs = mData.keys().toSet();

	foreach( QString refName, refs )
	{
		if( oldRefs.contains( refName ) )
		{
			oldRefs.remove( refName );
		}
		if( mData.contains( refName ) )
		{
			updateRef( refName );
		}
		else
		{
			addRef( refName );
		}
	}

	foreach( QString refName, oldRefs )
	{
		removeRef( refName );
	}
}

void RefsListModel::clear()
{
	beginResetModel();
	mData.clear();
	endResetModel();

	qDeleteAll( mOrderedData );
	mOrderedData.clear();
}
