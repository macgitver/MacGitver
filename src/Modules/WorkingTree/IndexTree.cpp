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

#include <QDateTime>
#include <QHeaderView>
#include <QFileIconProvider>
#include <QFileInfo>

#include <QDebug>

#include "IndexTree.h"

WorkingTreeNode::WorkingTreeNode( const QString& path, QTreeWidgetItem* item )
	: mPath( path )
	, mItem( item )
{
}

WorkingTreeNode::~WorkingTreeNode()
{
}

WorkingTreeFileNode::WorkingTreeFileNode( const QString& path, QTreeWidgetItem* item,
										  WorkingTreeDirNode* parent, IndexTree::TreeFilters state )
	: WorkingTreeNode( path, item )
	, mParent( parent )
	, mState( state )
{
	mParent->mChildren.append( this );
}

bool WorkingTreeFileNode::refilter( IndexTree* tree )
{
	if( mState & tree->filters() )
	{
		item()->setHidden( false );
		return true;
	}
	else
	{
		item()->setHidden( true );
		return false;
	}
}


WorkingTreeDirNode::WorkingTreeDirNode( const QString& path, QTreeWidgetItem* item,
										WorkingTreeDirNode* parent )
	: WorkingTreeNode( path, item )
	, mParent( parent )
{
	if( mParent )
	{
		mParent->mChildren.append( this );
	}
}

WorkingTreeDirNode::~WorkingTreeDirNode()
{
}

bool WorkingTreeDirNode::refilter( IndexTree* tree )
{
	bool anyChildVisible = false;

	for( int i = 0; i < mChildren.count(); i++ )
	{
		anyChildVisible |= mChildren.at( i )->refilter( tree );
	}

	if( !mParent )
	{
		return true;
	}

	if( anyChildVisible )
	{
		item()->setHidden( false );
		return true;
	}
	else
	{
		item()->setHidden( true );
		return false;
	}
}

IndexTree::IndexTree()
{
	mFilters = All;

	setColumnCount( 4 );
	setSortingEnabled( true );
	sortByColumn( 0, Qt::AscendingOrder );

	QTreeWidgetItem* head = headerItem();
	head->setData( 0, Qt::DisplayRole, trUtf8( "File" ) );
	head->setData( 1, Qt::DisplayRole, trUtf8( "Size" ) );
	head->setData( 2, Qt::DisplayRole, trUtf8( "M-Time" ) );
	head->setData( 3, Qt::DisplayRole, trUtf8( "Owner" ) );

	header()->setResizeMode( 0, QHeaderView::ResizeToContents );
}

void IndexTree::setRepository( const Git::Repository& repo )
{
	clear();
	mRepo = repo;
	if( mRepo.isValid() )
	{
		mIndex = mRepo.index();
	}
	else
	{
		mIndex = Git::Index();
	}

	update();
}

void IndexTree::clear()
{
	QTreeWidget::clear();

	qDeleteAll( mFileToNodes ); mFileToNodes.clear();
	qDeleteAll( mPathToNodes ); mPathToNodes.clear();

	mPathToNodes.insert( QString(), new WorkingTreeDirNode( QString(),
															invisibleRootItem(), NULL ) );
}

void IndexTree::update()
{
	QFileIconProvider ip;

	Git::StatusHash sh = mRepo.statusHash();
	Git::StatusHash::ConstIterator it = sh.constBegin();
	while( it != sh.constEnd() )
	{
		TreeFilters curState;

		unsigned int st = it.value();
		if( st == Git::StatusCurrent )
			curState |= Unchanged;

		else if( st & Git::StatusIgnored )
			curState |= Ignored;

		else if( st & Git::StatusWorkingTreeModified )
			curState |= Changed;

		else if( st & Git::StatusWorkingTreeNew )
			curState |= Untracked;

		else if( st & Git::StatusWorkingTreeDeleted )
			curState |= Missing;

		QString dir, file;
		int i = it.key().lastIndexOf( L'/' );
		if( i == -1 )
		{
			file = it.key();
		}
		else
		{
			dir = it.key().left( i );
			file = it.key().mid( i + 1);
		}

		WorkingTreeDirNode* dirNode = mPathToNodes.value( dir, NULL );

		if( !dirNode )
		{
			QString curPath;
			dirNode = mPathToNodes[ QString() ];

			foreach( QString part, dir.split( L'/' ) )
			{
				if( curPath.isEmpty() ) curPath = part; else curPath += L'/' + part;

				if( mPathToNodes.contains( curPath ) )
				{
					dirNode = mPathToNodes[ curPath ];
				}
				else
				{
					QTreeWidgetItem* me = new QTreeWidgetItem( dirNode->item(), QStringList( part ) );

					me->setData( 0, Qt::DecorationRole, ip.icon( QFileIconProvider::Folder ) );
					me->setData( 0, Qt::UserRole, "D" + part );

					QFileInfo fi( mRepo.basePath() + "/" + it.value() );
					me->setData( 3, Qt::DisplayRole, fi.owner() );

					dirNode = new WorkingTreeDirNode( curPath, me, dirNode );
					mPathToNodes.insert( curPath, dirNode );
				}
			}
		}

		WorkingTreeFileNode* fNode = mFileToNodes.value( it.key(), NULL );

		if( !fNode )
		{
			QTreeWidgetItem* item = new QTreeWidgetItem( dirNode->item(), QStringList( file ) );
			fNode = new WorkingTreeFileNode( it.key(), item, dirNode, curState );

			QFileInfo fi( mRepo.basePath() + "/" + it.key() );
			item->setData( 0, Qt::DecorationRole, ip.icon( fi ) );
			item->setData( 0, Qt::UserRole, "F" + file );
			item->setData( 1, Qt::DisplayRole, fi.size() );
			item->setData( 2, Qt::DisplayRole, fi.lastModified() );
			item->setData( 3, Qt::DisplayRole, fi.owner() );

			mFileToNodes.insert( it.key(), fNode );
		}

		if( curState & Unchanged )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::black );

		else if( curState & Changed )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::blue );

		else if( curState & Untracked )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::green );

		else if( curState & Missing )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::red );

		else if( curState & Ignored )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::gray );

		++it;
	}

	refilter();
}

void IndexTree::refilter()
{
	WorkingTreeDirNode* node = mPathToNodes.value( QString(), NULL );
	if( node )
	{
		node->refilter( this );
	}
}

void IndexTree::setFilter( TreeFilters filters )
{
	if( filters != mFilters )
	{
		mFilters = filters;
		refilter();
	}
}

IndexTree::TreeFilters IndexTree::filters() const
{
	return mFilters;
}
