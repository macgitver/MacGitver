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

#include "DiffList.h"
#include "DiffListPrivate.h"
#include "ObjectPrivate.h"
#include "ObjectTree.h"
#include "RepositoryPrivate.h"

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

		const git_tree_entry* entry = git_tree_entry_byname( d2, pathName.constData() );
		if( !entry )
		{
			return ObjectTree();
		}

		git_object* subObject = 0;
		int rc = git_tree_entry_to_object( &subObject, d->repo()->mRepo, entry );
		if( d->handleErrors( rc ) || !subObject )
		{
			return ObjectTree();
		}

		if( git_object_type( subObject ) != GIT_OBJ_TREE )
		{
			git_object_free( subObject );
			return ObjectTree();
		}

		return new ObjectPrivate( d->repo(), subObject );
	}

	DiffList ObjectTree::diffToTree( ObjectTree newTree )
	{
		if( !d )
		{
			return DiffList();
		}

		git_tree* gitTree = (git_tree*) d->mObj;
		git_tree* gitNewTree = (git_tree*) newTree.d->mObj;

		git_diff_list* diffList = NULL;
		int rc = git_diff_tree_to_tree( d->repo()->mRepo, NULL, gitTree, gitNewTree, &diffList );
		if( !d->handleErrors( rc ) )
		{
			return DiffList();
		}

		return DiffList( new DiffListPrivate( d->repo(), diffList ) );
	}

	DiffList ObjectTree::diffToIndex()
	{
		if( !d )
		{
			return DiffList();
		}

		git_tree* gitTree = (git_tree*) d->mObj;

		git_diff_list* diffList = NULL;
		int rc = git_diff_index_to_tree( d->repo()->mRepo, NULL, gitTree, &diffList );
		if( !d->handleErrors( rc ) )
		{
			return DiffList();
		}

		return DiffList( new DiffListPrivate( d->repo(), diffList ) );
	}

	DiffList ObjectTree::diffToWorkingDir()
	{
		if( !d )
		{
			return DiffList();
		}

		git_tree* gitTree = (git_tree*) d->mObj;

		git_diff_list* diffList = NULL;
		int rc = git_diff_workdir_to_tree( d->repo()->mRepo, NULL, gitTree, &diffList );
		if( !d->handleErrors( rc ) )
		{
			return DiffList();
		}

		return DiffList( new DiffListPrivate( d->repo(), diffList ) );
	}

}
