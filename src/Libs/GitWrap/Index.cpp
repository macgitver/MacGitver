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
#include "Index.h"
#include "Repository.h"
#include "RepositoryPrivate.h"
#include "IndexPrivate.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	IndexPrivate::IndexPrivate( RepositoryPrivate* repo, git_index* index )
		: RepoObject( repo )
		, mIndex( index )
	{
		Q_ASSERT( index );
	}

	IndexPrivate::~IndexPrivate()
	{
		Q_ASSERT( mRepo );
		if( mRepo->mIndex == this )
		{
			mRepo->mIndex = NULL;
		}

		if( mIndex )
		{
			git_index_free( mIndex );
		}
	}

	END_INTERNAL_IMPL()

	Index::Index()
	{
	}

	Index::Index( Internal::IndexPrivate* _d )
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

	bool Index::isValid() const
	{
		return d;
	}

	int Index::count( Result& result ) const
	{
		if( !result )
		{
			return 0;
		}

		if( !d )
		{
			result.setInvalidObject();
			return 0;
		}
		return git_index_entrycount( d->mIndex );
	}

	Repository Index::repository( Result& result ) const
	{
		if( !result )
		{
			return Repository();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Repository();
		}

		return Repository( d->repo() );
	}

	/**
	 * @brief			Read the index from storage
	 *
	 * Refills this index object with data obtained from hard disc. Any local modifications to this
	 * index object will be lost.
	 *
	 * @param[in,out]	result	A Result object; see @ref GitWrapErrorHandling
	 *
	 */
	void Index::read( Result& result )
	{
		if( !result )
		{
			return;
		}

		if( !d )
		{
			result.setInvalidObject();
			return;
		}

		result = git_index_read( d->mIndex );
	}

	/**
	 * @brief			Write the index to storage
	 *
	 * Writes this index object to the hard disc.
	 *
	 * @param[in,out]	result	A Result object; see @ref GitWrapErrorHandling
	 *
	 */
	void Index::write( Result& result )
	{
		if( !result )
		{
			return;
		}

		if( !d )
		{
			result.setInvalidObject();
			return;
		}

		result = git_index_write( d->mIndex );
	}

}
