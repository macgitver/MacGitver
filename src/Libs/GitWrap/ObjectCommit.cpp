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

#include "GitWrapPrivate.h"

#include "ObjectPrivate.h"
#include "ObjectCommit.h"
#include "ObjectTree.h"
#include "RepositoryPrivate.h"
#include "Reference.h"
#include "ReferencePrivate.h"

namespace Git
{

	ObjectCommit::ObjectCommit()
	{
	}

	ObjectCommit::ObjectCommit( Internal::ObjectPrivate* _d )
		: Object( _d )
	{
		Result r;
		if( ( type( r ) != otCommit ) || !r )
		{
			d = NULL;
		}
	}

	ObjectCommit::ObjectCommit( const ObjectCommit& o )
		: Object( o )
	{
	}

	ObjectTree ObjectCommit::tree( Result& result )
	{
		if( !result )
		{
			return ObjectTree();
		}
		if( !d )
		{
			result.setInvalidObject();
			return ObjectTree();
		}

		git_commit* commit = (git_commit*) d->mObj;
		git_tree* tree = 0;

		result = git_commit_tree( &tree, commit );
		if( !result )
		{
			return ObjectTree();
		}

		return new Internal::ObjectPrivate( d->repo(), (git_object*) tree );
	}

	ObjectId ObjectCommit::treeId( Result& result )
	{
		if( !result )
		{
			return ObjectId();
		}
		if( !d )
		{
			result.setInvalidObject();
			return ObjectId();
		}

		git_commit* commit = (git_commit*) d->mObj;

		return ObjectId::fromRaw( git_commit_tree_oid( commit )->id );
	}

	ObjectIdList ObjectCommit::parentCommitIds( Result& result ) const
	{
		ObjectIdList ids;

		if( !result )
		{
			return ids;
		}
		if( !d )
		{
			result.setInvalidObject();
			return ids;
		}

		git_commit* commit = (git_commit*) d->mObj;

		for( unsigned int i = 0; i < numParentCommits(); i++ )
		{
			ObjectId id = ObjectId::fromRaw( git_commit_parent_oid( commit, i )->id );
			ids.append( id );
		}

		return ids;
	}

	ObjectCommit ObjectCommit::parentCommit( unsigned int index, Result& result ) const
	{
		if( !result )
		{
			return ObjectCommit();
		}
		if( !d )
		{
			result.setInvalidObject();
			return ObjectCommit();
		}

		git_commit* commit = (git_commit*) d->mObj;
		git_commit* gitparent = NULL;

		result = git_commit_parent( &gitparent, commit, index );
		if( !result )
		{
			return ObjectCommit();
		}

		return new Internal::ObjectPrivate( d->repo(), (git_object*) gitparent );
	}

	ObjectId ObjectCommit::parentCommitId( unsigned int index, Result& result ) const
	{
		if( !result )
		{
			return ObjectId();
		}
		if( !d )
		{
			result.setInvalidObject();
			return ObjectId();
		}

		if( numParentCommits() > index )
		{
			git_commit* commit = (git_commit*) d->mObj;

			const git_oid* oid = git_commit_parent_oid( commit, index );
			if( oid )
			{
				return ObjectId::fromRaw( oid->id );
			}
		}

		return ObjectId();
	}

	QList< ObjectCommit > ObjectCommit::parentCommits( Result& result ) const
	{
		if( !result )
		{
			return QList< ObjectCommit >();
		}
		if( !d )
		{
			result.setInvalidObject();
			return QList< ObjectCommit >();
		}

		QList< ObjectCommit > objs;

		git_commit* commit = (git_commit*) d->mObj;

		for( unsigned int i = 0; i < numParentCommits(); i++ )
		{
			git_commit* parent = NULL;

			result = git_commit_parent( &parent, commit, i );
			if( !result )
			{
				return QList< ObjectCommit >();
			}

			objs.append( new Internal::ObjectPrivate( d->repo(), (git_object*) parent ) );
		}

		return objs;
	}

	unsigned int ObjectCommit::numParentCommits() const
	{
		if( !d )
		{
			return 0;
		}

		git_commit* commit = (git_commit*) d->mObj;
		return git_commit_parentcount( commit );
	}

	bool ObjectCommit::isParentOf( const Git::ObjectCommit& child, Result& result ) const
	{
		QList< Git::ObjectCommit > parents = child.parentCommits( result );

		for( int i = 0; result && i < parents.count(); i++ )
		{
			if( isEqual( parents[ i ], result ) )
				return true;
		}

		return false;
	}

	bool ObjectCommit::isChildOf( const Git::ObjectCommit& parent, Result& result ) const
	{
		QList< Git::ObjectCommit > children = parentCommits( result );

		for( int i = 0; result && i < children.count(); i++ )
		{
			if( parent.isEqual( children[ i ], result ) )
			{
				return true;
			}
		}

		return false;
	}

	bool ObjectCommit::isEqual( const Git::ObjectCommit& commit, Result& result ) const
	{
		return id( result ) == commit.id( result ) && result;
	}

	Signature ObjectCommit::author( Result& result ) const
	{
		if( !result )
		{
			return Signature();
		}
		if( !d )
		{
			result.setInvalidObject();
			return Signature();
		}

		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_author( commit );

		return Internal::git2Signature( sig );
	}

	Signature ObjectCommit::committer( Result& result ) const
	{
		if( !result )
		{
			return Signature();
		}
		if( !d )
		{
			result.setInvalidObject();
			return Signature();
		}

		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_committer( commit );

		return Internal::git2Signature( sig );
	}

	QString ObjectCommit::message( Result& result ) const
	{
		if( !result )
		{
			return QString();
		}

		if( !d )
		{
			result.setInvalidObject();
			return QString();
		}

		git_commit* commit = (git_commit*) d->mObj;
		const char* msg = git_commit_message( commit );
		int len = int( strlen( msg ) );

		if( len && msg[ len - 1 ] == '\n' )
		{
			len--;
		}

		return QString::fromUtf8( msg, len );
	}

	QString ObjectCommit::shortMessage( Result& result ) const
	{
		if( !result )
		{
			return QString();
		}

		if( !d )
		{
			result.setInvalidObject();
			return QString();
		}

		git_commit* commit = (git_commit*) d->mObj;
		const char* msg = git_commit_message( commit );

		int len = 0;
		while( msg[ len ] && msg[ len ] != '\n' )
		{
			len++;
		}

		return QString::fromUtf8( msg, len );
	}

	Reference ObjectCommit::createBranch( const QString& name, bool force, Result& result )
	{
		if( !result )
		{
			return Reference();
		}
		if( !d )
		{
			result.setInvalidObject();
			return Reference();
		}

		git_reference* ref = NULL;
		result = git_branch_create( &ref, d->repo()->mRepo, name.toUtf8().constData(),
									d->mObj, force );
		if( !result )
		{
			return Reference();
		}

		return Reference( new Internal::ReferencePrivate( d->repo(), ref ) );
	}

}
