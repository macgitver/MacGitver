
#include <QFileIconProvider>
#include <QFileInfo>

#include "git2/status.h"

#include <QDebug>

#include "Views/WorkingTree/IndexTree.h"

WorkingTreeNode::WorkingTreeNode( const QString& path, QTreeWidgetItem* item )
	: mPath( path )
	, mItem( item )
{
}

WorkingTreeFileNode::WorkingTreeFileNode( const QString& path, QTreeWidgetItem* item )
	: WorkingTreeNode( path, item )
{
}


WorkingTreeDirNode::WorkingTreeDirNode( const QString& path, QTreeWidgetItem* item,
										bool deleteItem )
	: WorkingTreeNode( path, item )
{
	mDeleteItem = deleteItem;
}

WorkingTreeDirNode::~WorkingTreeDirNode()
{
	if( mDeleteItem )
		delete item();
}

IndexTree::IndexTree()
{
	setSortingEnabled( true );
}

void IndexTree::setRepository( const Git::Repository& repo )
{
	clear();
	mRepo = repo;
	mIndex = mRepo.index();

	update();
}

void IndexTree::clear()
{
	qDeleteAll( mFileToNodes );
	qDeleteAll( mPathToNodes );

	mPathToNodes.insert( QString(), new WorkingTreeDirNode( QString(), invisibleRootItem(), false ) );
}

void IndexTree::update()
{
	QFileIconProvider ip;

	Git::StatusHash sh = mRepo.statusHash();
	Git::StatusHash::ConstIterator it = sh.constBegin();
	while( it != sh.constEnd() )
	{
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

					dirNode = new WorkingTreeDirNode( curPath, me );
					mPathToNodes.insert( curPath, dirNode );
				}
			}
		}

		WorkingTreeFileNode* fNode = mFileToNodes.value( it.key(), NULL );

		if( !fNode )
		{
			QTreeWidgetItem* item = new QTreeWidgetItem( dirNode->item(), QStringList( file ) );
			fNode = new WorkingTreeFileNode( it.key(), item );

			QFileInfo fi( mRepo.basePath() + "/" + it.key() );
			item->setData( 0, Qt::DecorationRole, ip.icon( fi ) );
			item->setData( 0, Qt::UserRole, "F" + file );

			mFileToNodes.insert( it.key(), fNode );
		}

		unsigned int st = it.value();
		if( st == 0 )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::black );
		else if( st & GIT_STATUS_WT_MODIFIED )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::blue );
		else if( st & GIT_STATUS_WT_NEW )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::green );
		else if( st & GIT_STATUS_WT_DELETED )
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::red );

		++it;
	}
}
