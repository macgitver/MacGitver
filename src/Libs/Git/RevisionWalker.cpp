
#include "Libs/Git/RevisionWalker.h"
#include "Libs/Git/ObjectId.h"
#include "Libs/Git/Reference.h"
#include "Libs/Git/Git_p.h"

namespace Git
{

	RevisionWalkerPrivate::RevisionWalkerPrivate( RepositoryPrivate* repo, git_revwalk* walker )
		: mRepo( repo )
		, mWalker( walker )
	{
		Q_ASSERT( mRepo );
		mRepo->ref();
	}

	RevisionWalkerPrivate::~RevisionWalkerPrivate()
	{
		if( mRepo )
		{
			mRepo->deref();
		}

		git_revwalk_free( mWalker );
	}

	RevisionWalker::RevisionWalker()
	{
	}

	RevisionWalker::RevisionWalker( RevisionWalkerPrivate* _d )
		: d( _d )
	{
	}

	RevisionWalker::~RevisionWalker()
	{
	}

	RevisionWalker& RevisionWalker::operator=( const RevisionWalker& other )
	{
		d = other.d;
		return * this;
	}

	bool RevisionWalker::isValid() const
	{
		return d;
	}

	void RevisionWalker::reset()
	{
		Q_ASSERT( d );
		git_revwalk_reset( d->mWalker );
	}

	void RevisionWalker::push( const ObjectId& id )
	{
		Q_ASSERT( d );
		if( d )
		{
			git_revwalk_push( d->mWalker, (const git_oid*) id.raw() );
		}
	}

	void RevisionWalker::push( const Reference& ref )
	{
		pushRef( ref.name() );
	}

	void RevisionWalker::pushRef( const QByteArray& name )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_push_ref( d->mWalker, name.constData() );
			Q_ASSERT( rc == GIT_SUCCESS );
		}
	}

	void RevisionWalker::pushHead()
	{
		Q_ASSERT( d );
		if( d )
		{
			git_revwalk_push_head( d->mWalker );
		}
	}

	bool RevisionWalker::next( ObjectId& oidNext )
	{
		Q_ASSERT( d );

		if( d )
		{
			git_oid oid;
			int rc = git_revwalk_next( &oid, d->mWalker );
			if( rc == GIT_SUCCESS )
			{
				oidNext = ObjectId::fromRaw( oid.id );
				return true;
			}
		}

		const git_error* err = giterr_last();
		qDebug( "GIT-Error:\n%s\n\n", err->message );

		return false;
	}

	void RevisionWalker::setSorting( bool topological, bool timed )
	{
		Q_ASSERT( d );

		if( d )
		{
			git_revwalk_sorting( d->mWalker,
								 ( topological ? GIT_SORT_TOPOLOGICAL : 0 ) |
								 ( timed ? GIT_SORT_TIME : 0 ) );
		}
	}

}

