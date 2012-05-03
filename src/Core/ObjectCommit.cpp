
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
