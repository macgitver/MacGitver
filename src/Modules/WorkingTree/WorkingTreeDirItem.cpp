
#include <QVariant>

#include "WorkingTreeDirItem.h"
#include "WorkingTreeFileItem.h"

WorkingTreeDirItem::WorkingTreeDirItem( WorkingTreeAbstractItem* parent )
	: mParent( parent )
{
}

WorkingTreeDirItem::~WorkingTreeDirItem()
{
}

int WorkingTreeDirItem::visibleChildren() const
{
	int i = mVisibleDirs.count() + mVisibleFiles.count();
//	qDebug( "visChildren=%i", i );
	return i;
}

WorkingTreeAbstractItem* WorkingTreeDirItem::visibleChildAt( int index )
{
	if( index < mVisibleDirs.count() )
	{
		return mVisibleDirs.at( index );
	}

	index -= mVisibleDirs.count();
	if( index < mVisibleFiles.count() )
	{
		return mVisibleFiles.at( index );
	}

	return NULL;
}

QVariant WorkingTreeDirItem::data( int column, int role ) const
{
	switch( role )
	{
	case Qt::DisplayRole:
		switch( column )
		{
		case 0:		return mName;
		}
		break;

	case Qt::DecorationRole:
		switch( column )
		{
		case 0:		return mIcon;
		}
		break;

	default:
		break;
	}

	return QVariant();
}

WorkingTreeAbstractItem* WorkingTreeDirItem::parent()
{
	return mParent;
}

int WorkingTreeDirItem::visibleIndex() const
{
	return 0;
}

WorkingTreeAbstractItem* WorkingTreeDirItem::childByName( const QString& name )
{
	return mChildren.value( name, NULL );
}

void WorkingTreeDirItem::appendItem( WorkingTreeAbstractItem* item )
{
	mChildren.insert( item->name(), item );
}

bool WorkingTreeDirItem::refilter( WorkingTreeFilters filters )
{
	mVisibleDirs.clear();
	mVisibleFiles.clear();

	bool anyVisible = false;

	foreach( WorkingTreeAbstractItem* item, mChildren )
	{
		bool visible = item->refilter( filters );
		if( visible )
		{
			anyVisible = true;
			if( item->isDirectory() )
			{
				mVisibleDirs.append( (WorkingTreeDirItem*) item );
			}
			else
			{
				mVisibleFiles.append( (WorkingTreeFileItem*) item );
			}
		}
	}

	return anyVisible;
}

void WorkingTreeDirItem::setIcon( const QIcon& icon )
{
	mIcon = icon;
}

void WorkingTreeDirItem::setName( const QString& name )
{
	mName = name;
}

QString WorkingTreeDirItem::name() const
{
	return mName;
}

bool WorkingTreeDirItem::isDirectory() const
{
	return true;
}

void WorkingTreeDirItem::removeChild( WorkingTreeAbstractItem* child )
{
	mChildren.remove( child->name() );

	for( int i = 0; i < mVisibleDirs.count(); i++ )
	{
		if( mVisibleDirs[ i ] == child )
		{
			mVisibleDirs.removeAt( i );
			break;
		}
	}

	for( int i = 0; i < mVisibleFiles.count(); i++ )
	{
		if( mVisibleFiles[ i ] == child )
		{
			mVisibleFiles.removeAt( i );
			break;
		}
	}
}
