
#include <QVariant>

#include "WorkingTreeDirItem.h"
#include "WorkingTreeFileItem.h"
#include "WorkingTreeModel.h"

WorkingTreeDirItem::WorkingTreeDirItem( WorkingTreeModel* model, WorkingTreeAbstractItem* parent )
	: WorkingTreeAbstractItem( model, parent )
{
}

WorkingTreeDirItem::~WorkingTreeDirItem()
{
	qDeleteAll( mFiles );
	qDeleteAll( mDirs );
	mFiles.clear();
	mDirs.clear();
	mChildren.clear();
}

int WorkingTreeDirItem::visibleChildren() const
{
	int visible = 0;

	for( int i = 0; i < mDirs.count(); i++ )
	{
		if( mDirs[ i ]->isVisible() )
			visible++;
	}

	for( int i = 0; i < mFiles.count(); i++ )
	{
		if( mFiles[ i ]->isVisible() )
			visible++;
	}

	return visible;
}

WorkingTreeAbstractItem* WorkingTreeDirItem::visibleChildAt( int index )
{
	for( int i = 0; i < mChildren.count(); i++ )
	{
		WorkingTreeAbstractItem* c = childAt( i );

		if( c->isVisible() )
		{
			if( !index )
				return c;
			else
				--index;
		}
	}

	return NULL;
}

WorkingTreeAbstractItem* WorkingTreeDirItem::childAt( int index )
{
	if( index < mDirs.count() )
	{
		return mDirs.at( index );
	}

	index -= mDirs.count();
	if( index < mFiles.count() )
	{
		return mFiles.at( index );
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

int WorkingTreeDirItem::visibleIndex() const
{
	if( !mParent )
		return -1;

	int numVis = mParent->visibleChildren();
	for( int i = 0; i < numVis; i++ )
	{
		if( mParent->visibleChildAt( i ) == this )
		{
			return i;
		}
	}

	return -1;
}

WorkingTreeAbstractItem* WorkingTreeDirItem::childByName( const QString& name )
{
	return mChildren.value( name, NULL );
}

void WorkingTreeDirItem::appendItem( WorkingTreeAbstractItem* item )
{
	// No model to update at this place, new items are _always_ hidden by default

	if( item->isDirectory() )
	{
		int index = 0;
		while( index < mDirs.count() && mDirs[ index ]->name() < item->name() )
			index++;
		mDirs.insert( index, (WorkingTreeDirItem*) item );
	}
	else
	{
		int index = 0;
		while( index < mFiles.count() && mFiles[ index ]->name() < item->name() )
			index++;
		mFiles.insert( index, (WorkingTreeFileItem*) item );
	}

	mChildren.insert( item->name(), item );
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
	int i = child->visibleIndex();

	if( i != -1 )
	{
		model()->beginRemoveRows( index(), i, i );
	}
	mChildren.remove( child->name() );

	if( child->isDirectory() )
	{
		for( int i = 0; i < mDirs.count(); i++ )
		{
			if( mDirs[ i ] == child )
			{
				mDirs.remove( i );
				break;
			}
		}
	}
	else
	{
		for( int i = 0; i < mFiles.count(); i++ )
		{
			if( mFiles[ i ] == child )
			{
				mFiles.remove( i );
				break;
			}
		}
	}

	delete child;

	if( i != -1 )
	{
		model()->endRemoveRows();
	}
}
