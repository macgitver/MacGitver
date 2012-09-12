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
		Q_ASSERT( type() == otCommit );
	}

	ObjectCommit::ObjectCommit( const ObjectCommit& o )
		: Object( o )
	{
	}

	ObjectTree ObjectCommit::tree()
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;

		git_tree* tree = 0;

		int rc = git_commit_tree( &tree, commit );
		if( !d->handleErrors( rc ) )
		{
			return ObjectTree();
		}

		return new Internal::ObjectPrivate( d->repo(), (git_object*) tree );
	}

	ObjectId ObjectCommit::treeId()
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;

		return ObjectId::fromRaw( git_commit_tree_oid( commit )->id );
	}

	ObjectIdList ObjectCommit::parentCommitIds() const
	{
		ObjectIdList ids;

		if( d )
		{
			git_commit* commit = (git_commit*) d->mObj;

			for( unsigned int i = 0; i < numParentCommits(); i++ )
			{
				ObjectId id = ObjectId::fromRaw( git_commit_parent_oid( commit, i )->id );
				ids.append( id );
			}
		}

		return ids;
	}

	ObjectCommit ObjectCommit::parentCommit( unsigned int index ) const
	{
		ObjectCommit parent;

		if( d && numParentCommits() > index )
		{
			git_commit* commit = (git_commit*) d->mObj;
			git_commit* gitparent = NULL;

			int rc = git_commit_parent( &gitparent, commit, index );
			if( d->handleErrors( rc ) )
			{
				parent = new Internal::ObjectPrivate( d->repo(), (git_object*) gitparent );
			}
		}

		return parent;
	}

	ObjectId ObjectCommit::parentCommitId( unsigned int index ) const
	{
		if( d && numParentCommits() > index )
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

	QList< ObjectCommit > ObjectCommit::parentCommits() const
	{
		QList< ObjectCommit > objs;

		if( d )
		{
			git_commit* commit = (git_commit*) d->mObj;

			for( unsigned int i = 0; i < numParentCommits(); i++ )
			{
				git_commit* parent = NULL;

				int rc = git_commit_parent( &parent, commit, i );
				if( !d->handleErrors( rc ) )
				{
					return QList< ObjectCommit >();
				}

				objs.append( new Internal::ObjectPrivate( d->repo(), (git_object*) parent ) );
			}
		}

		return objs;
	}

	unsigned int ObjectCommit::numParentCommits() const
	{
		if( d )
		{
			git_commit* commit = (git_commit*) d->mObj;

			return git_commit_parentcount( commit );
		}

		return 0;
	}

	bool ObjectCommit::isParentOf( const Git::ObjectCommit& child ) const
	{
		if( d )
		{
			QList< Git::ObjectCommit > parents = child.parentCommits();

			for( int i = 0; i < parents.count(); i++ )
			{
				if( isEqual( parents[ i ] ) )
					return true;
			}
		}

		return false;
	}

	bool ObjectCommit::isChildOf( const Git::ObjectCommit& parent ) const
	{
		if( d )
		{
			QList< Git::ObjectCommit > children = parentCommits();

			for( int i = 0; i < children.count(); i++ )
			{
				if( parent.isEqual( children[ i ] ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool ObjectCommit::isEqual( const Git::ObjectCommit& commit ) const
	{
		return id() == commit.id();
	}

	Signature ObjectCommit::author() const
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_author( commit );
		return Internal::git2Signature( sig );
	}

	Signature ObjectCommit::committer() const
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_committer( commit );
		return Internal::git2Signature( sig );
	}

	QString ObjectCommit::message() const
	{
		Q_ASSERT( d );

		git_commit* commit = (git_commit*) d->mObj;
		const char* msg = git_commit_message( commit );
		int len = int( strlen( msg ) );

		if( len && msg[ len - 1 ] == '\n' )
			len--;

		return QString::fromUtf8( msg, len );
	}

	QString ObjectCommit::shortMessage() const
	{
		Q_ASSERT( d );

		git_commit* commit = (git_commit*) d->mObj;
		const char* msg = git_commit_message( commit );

		int len = 0;
		while( msg[ len ] && msg[ len ] != '\n' )
			len++;

		return QString::fromUtf8( msg, len );
	}

	Reference ObjectCommit::createBranch( const QString& name, bool force )
	{
		if( !d )
		{
			return Reference();
		}

		git_reference* ref = NULL;
		int rc = git_branch_create( &ref, d->repo()->mRepo, name.toUtf8().constData(),
									d->mObj, force );
		if( !d->handleErrors( rc ) )
		{
			return Reference();
		}

		return Reference( new Internal::ReferencePrivate( d->repo(), ref ) );
	}

}
