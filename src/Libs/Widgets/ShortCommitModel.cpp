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

#include "libGitWrap/Repository.h"
#include "libGitWrap/Result.h"

#include "ShortCommitModel.h"

ShortCommitModel::ShortCommitModel( QObject* parent )
	: QAbstractTableModel( parent )
{
}

ShortCommitModel::~ShortCommitModel()
{
}

void ShortCommitModel::clear()
{
	beginResetModel();

	mCommitList.clear();

	endResetModel();
}

void ShortCommitModel::setCommitList( const QList< Git::ObjectCommit >& list )
{
	beginResetModel();

	mCommitList = list;

	endResetModel();
}

bool ShortCommitModel::setCommitList( Git::Repository repo, const QList< Git::ObjectId >& list )
{
	QList< Git::ObjectCommit > commits;

	Git::Result r;

	foreach( Git::ObjectId sha1, list )
	{
		Git::ObjectCommit commit = repo.lookupCommit( sha1, r );
		if( !r )
		{
			return false;
		}
		commits.append( commit );
	}

	setCommitList( commits );
	return true;
}

Git::ObjectCommit ShortCommitModel::indexToCommit( const QModelIndex& index ) const
{
	if( index.isValid() )
	{
		return mCommitList.at( index.row() );
	}
	return Git::ObjectCommit();
}

int ShortCommitModel::rowCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : mCommitList.count();
}

int ShortCommitModel::columnCount( const QModelIndex& parent ) const
{
	return parent.isValid() ? 0 : 3;
}

QVariant ShortCommitModel::data( const QModelIndex& index, int role ) const
{
	QVariant value;
	Git::Result r;

	if( role == Qt::DisplayRole )
	{
		Git::ObjectCommit commit = indexToCommit( index );
		switch( index.column() )
		{
		case 0:
			value = commit.shortMessage( r );
			break;

		case 1:
			value = commit.author( r ).when().toString();
			break;

		case 2:
			value = commit.id( r ).toString().left( 7 );
			break;

		default:
			return QVariant();
		}
	}

	if( !r )
	{
		return QVariant();
	}

	return value;
}
