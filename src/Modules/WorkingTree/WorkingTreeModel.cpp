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

#include <QFileIconProvider>
#include <QFileInfo>

#include "WorkingTreeModel.h"
#include "WorkingTreeAbstractItem.h"
#include "WorkingTreeDirItem.h"
#include "WorkingTreeFileItem.h"

WorkingTreeModel::WorkingTreeModel( Git::Repository repo, QObject* parent )
	: QAbstractItemModel( parent )
	, mRepo( repo )
	, mRootItem( NULL )
{
	mFilters = WTF_All;
	mRootItem = new WorkingTreeDirItem( this, NULL );
	update();
}

WorkingTreeModel::~WorkingTreeModel()
{
	delete mRootItem;
}

QVariant WorkingTreeModel::data( const QModelIndex& index, int role ) const
{
	if( !index.isValid() )
	{
		return QVariant();
	}

	WorkingTreeAbstractItem* item =
			static_cast< WorkingTreeAbstractItem* >( index.internalPointer() );

	return item->data( index.column(), role );
}

Qt::ItemFlags WorkingTreeModel::flags( const QModelIndex& index ) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant WorkingTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( orientation == Qt::Vertical || role != Qt::DisplayRole )
	{
		return QVariant();
	}

	switch( section )
	{
	case 0:		return trUtf8( "Name" );
	case 1:		return trUtf8( "Size" );
	case 2:		return trUtf8( "M-Time" );
	case 3:		return trUtf8( "Owner" );
	default:	return QVariant();
	}
}

QModelIndex WorkingTreeModel::index( int row, int column, const QModelIndex& parent ) const
{
	if( !hasIndex( row, column, parent ) )
	{
		return QModelIndex();
	}

	WorkingTreeAbstractItem* parentItem;

	if( !parent.isValid() )
		parentItem = mRootItem;
	else
		parentItem = static_cast<WorkingTreeAbstractItem*>( parent.internalPointer() );

	WorkingTreeAbstractItem* childItem = parentItem->visibleChildAt( row );
	if( childItem )
		return createIndex( row, column, childItem );
	else
		return QModelIndex();
}

QModelIndex WorkingTreeModel::parent( const QModelIndex& index ) const
{
	if( !index.isValid() )
		return QModelIndex();

	WorkingTreeAbstractItem* childItem =
			static_cast< WorkingTreeAbstractItem* >( index.internalPointer() );
	WorkingTreeAbstractItem* parentItem = childItem->parent();

	if( parentItem == mRootItem )
		return QModelIndex();

	return createIndex( parentItem->visibleIndex(), 0, parentItem );
}

int WorkingTreeModel::rowCount( const QModelIndex& parent ) const
{
	WorkingTreeAbstractItem* parentItem;
	if( parent.column() > 0 )
	{
		return 0;
	}

	if( !parent.isValid() )
		parentItem = mRootItem;
	else
		parentItem = static_cast< WorkingTreeAbstractItem* >( parent.internalPointer() );

	return parentItem->visibleChildren();
}

int WorkingTreeModel::columnCount( const QModelIndex& parent ) const
{
	return 4;
}

void WorkingTreeModel::setRepository( Git::Repository repo )
{
	mRepo = repo;
	update();
}

void WorkingTreeModel::update()
{
	QFileIconProvider ip;
	QIcon folderIcon = ip.icon( QFileIconProvider::Folder );

	Git::StatusHash sh = mRepo.statusHash();
	Git::StatusHash::ConstIterator it = sh.constBegin();
	while( it != sh.constEnd() )
	{
		WorkingTreeFilters curState;

		unsigned int st = it.value();
		if( st == Git::StatusCurrent )
			curState |= WTF_Unchanged;

		else if( st & Git::StatusIgnored )
			curState |= WTF_Ignored;

		else if( st & Git::StatusWorkingTreeModified )
			curState |= WTF_Changed;

		else if( st & Git::StatusWorkingTreeNew )
			curState |= WTF_Untracked;

		else if( st & Git::StatusWorkingTreeDeleted )
			curState |= WTF_Missing;

		#if 0
		else if( st & Git::StatusIndexModified )
			curState |= Changed;

		else if( st & Git::StatusIndexNew )
			curState |= Untracked;

		else if( st & Git::StatusIndexDeleted )
			curState |= Missing;
		#endif

		WorkingTreeDirItem* cur = mRootItem;
		QStringList slNames = it.key().split( L'/' );
		while( slNames.count() > 1 )
		{
			QString dir = slNames.takeFirst();
			WorkingTreeDirItem* next = (WorkingTreeDirItem*) cur->childByName( dir );
			if( !next )
			{
				next = new WorkingTreeDirItem( this, cur );
				next->setName( dir );
				next->setIcon( folderIcon );
				cur->appendItem( next );
			}
			Q_ASSERT( next->isDirectory() );
			cur = (WorkingTreeDirItem*) next;
		}

		Q_ASSERT( slNames.count() == 1 );
		WorkingTreeAbstractItem* fileBase = cur->childByName( slNames[ 0 ] );
		WorkingTreeFileItem* file;
		if( fileBase && fileBase->isDirectory() )
		{
			cur->removeChild( fileBase );
			fileBase = NULL;
		}

		if( !fileBase )
		{
			file = new WorkingTreeFileItem( this, cur );
			file->setName( slNames[ 0 ] );
			cur->appendItem( file );
			fileBase = file;
		}

		Q_ASSERT( fileBase && !fileBase->isDirectory() );
		file = (WorkingTreeFileItem*) fileBase;

		QFileInfo fi( mRepo.basePath() + L'/' + it.key() );

		file->setIcon( ip.icon( fi ) );
		file->setSize( fi.size() );
		file->setLastModified( fi.lastModified() );
		file->setOwner( fi.owner() );

		WorkingTreeFilter state = WorkingTreeFilter( int( curState ) );
		file->setState( state, mFilters & state );

		++it;
	}

	// TODO: Remove all not visited items (including empty dirs)
}

void WorkingTreeModel::setFilters( WorkingTreeFilters filters )
{
	mFilters = filters;
	update();
	// mRootItem->refilter( mFilters );
}
