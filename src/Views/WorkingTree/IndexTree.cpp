
#include <QDateTime>
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
#if 0
	// Don't delete any items. They're freed from IndexTree::clear()
	if( mDeleteItem )
	{
		QTreeWidgetItem* it = item();
		delete it;
	}
#endif
}

IndexTree::IndexTree()
{
	setSortingEnabled( true );
	setColumnCount( 3 );
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

					QFileInfo fi( mRepo.basePath() + "/" + it.value() );
					me->setData( 2, Qt::DisplayRole, fi.lastModified() );

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
			item->setData( 2, Qt::DisplayRole, fi.lastModified() );
			item->setData( 1, Qt::DisplayRole, fi.size() );

			mFileToNodes.insert( it.key(), fNode );
		}

		if( mRepo.shouldIgnore( it.key() ) )
		{
			fNode->item()->setData( 0, Qt::ForegroundRole, Qt::darkGray );
		}
		else
		{
			unsigned int st = it.value();
			if( st == 0 )
				fNode->item()->setData( 0, Qt::ForegroundRole, Qt::black );
			else if( st & GIT_STATUS_WT_MODIFIED )
				fNode->item()->setData( 0, Qt::ForegroundRole, Qt::blue );
			else if( st & GIT_STATUS_WT_NEW )
				fNode->item()->setData( 0, Qt::ForegroundRole, Qt::green );
			else if( st & GIT_STATUS_WT_DELETED )
				fNode->item()->setData( 0, Qt::ForegroundRole, Qt::red );
		}
		++it;
	}
}
