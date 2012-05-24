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
#include "Index.h"
#include "Repository.h"

namespace Git
{

	IndexPrivate::IndexPrivate( RepositoryPrivate* repo, git_index* index )
		: mRepo( repo )
		, mIndex( index )
	{
		Q_ASSERT( mRepo );
		mRepo->ref();
	}

	IndexPrivate::~IndexPrivate()
	{
		Q_ASSERT( mRepo );
		if( mRepo->mIndex == this )
		{
			mRepo->mIndex = NULL;
		}

		mRepo->deref();

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

	Repository Index::repository() const
	{
		return Repository( d ? d->mRepo : NULL );
	}

}
