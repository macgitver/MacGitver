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

#include "Git_p.h"

#include "ObjectPrivate.h"
#include "ObjectTree.h"

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
		if( d->handleErrors( rc ) )
		{
			return ObjectTree();
		}

		return new ObjectPrivate( d->repo(), (git_object*) subTree );
	}

}
