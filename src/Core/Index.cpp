
#include "Core/Git_p.h"
#include "Core/Index.h"

namespace Git
{

	IndexPrivate::IndexPrivate( RepositoryPrivate* repo, git_index* index )
		: mRepo( repo )
		, mIndex( index )
	{
	}

	IndexPrivate::~IndexPrivate()
	{
		if( mIndex )
		{
			git_index_free( mIndex );
		}
	}

	Index::Index()
	{
	}

	Index::Index( IndexPrivate* _d )
		: d( _d )
	{
	}

	Index::Index( const Index& o )
		: d( o.d )
	{
	}

	Index::~Index()
	{
	}

	Index& Index::operator=( const Index& other )
	{
		d = other.d;
		return *this;
	}

	int Index::count() const
	{
		Q_ASSERT( d );
		return git_index_entrycount( d->mIndex );
	}

	bool Index::isValid() const
	{
		return d;
	}

}