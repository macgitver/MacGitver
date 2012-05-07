
#include "Core/Git_p.h"

#include "Core/ObjectCommit.h"
#include "Core/ObjectTree.h"

namespace Git
{

	ObjectCommit::ObjectCommit()
	{
	}

	ObjectCommit::ObjectCommit( ObjectPrivate* _d )
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
		if( rc < GIT_SUCCESS )
		{
			return ObjectTree();
		}

		return new ObjectPrivate( d->mRepo, (git_object*) tree );
	}

	ObjectId ObjectCommit::treeId()
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;

		return ObjectId::fromRaw( git_commit_tree_oid( commit )->id );
	}

	QList< ObjectId > ObjectCommit::parentCommitIds()
	{
		QList< ObjectId > ids;

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

	QList< ObjectCommit > ObjectCommit::parentCommits()
	{
		QList< ObjectCommit > objs;

		if( d )
		{
			git_commit* commit = (git_commit*) d->mObj;

			for( unsigned int i = 0; i < numParentCommits(); i++ )
			{
				git_commit* parent = NULL;

				int rc = git_commit_parent( &parent, commit, i );
				if( rc == -1 )
				{
					return QList< ObjectCommit >();
				}

				objs.append( new ObjectPrivate( d->mRepo, (git_object*) parent ) );
			}
		}

		return objs;
	}

	unsigned int ObjectCommit::numParentCommits()
	{
		if( d )
		{
			git_commit* commit = (git_commit*) d->mObj;

			return git_commit_parentcount( commit );
		}

		return 0;
	}

	Signature ObjectCommit::author()
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_author( commit );
		return git2Signature( sig );
	}

	Signature ObjectCommit::committer()
	{
		Q_ASSERT( d );
		git_commit* commit = (git_commit*) d->mObj;
		const git_signature* sig = git_commit_committer( commit );
		return git2Signature( sig );
	}

}
