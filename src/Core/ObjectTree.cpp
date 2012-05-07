
#include "Core/Git_p.h"

#include "Core/ObjectTree.h"

namespace Git
{

	ObjectTree::ObjectTree()
	{
	}

	ObjectTree::ObjectTree( ObjectPrivate* _d )
		: Object( _d )
	{
		Q_ASSERT( type() == otTree );
	}

	ObjectTree::ObjectTree( const ObjectTree& o )
		: Object( o )
	{
	}

	ObjectTree ObjectTree::subPath( const QByteArray& pathName ) const
	{
		Q_ASSERT( d );
		git_tree* d2 = (git_tree*) d->mObj;

		git_tree* subTree = 0;

		int rc = git_tree_get_subtree( &subTree, d2, pathName.constData() );
		if( rc < GIT_SUCCESS )
		{
			return ObjectTree();
		}

		return new ObjectPrivate( d->mRepo, (git_object*) subTree );
	}

}
