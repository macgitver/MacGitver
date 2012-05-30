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
#include "ChangeListConsumer.h"
#include "DiffList.h"
#include "Repository.h"
#include "RepositoryPrivate.h"
#include "DiffListPrivate.h"

namespace Git
{

	DiffListPrivate::DiffListPrivate( RepositoryPrivate* repo, git_diff_list* difflist )
		: RepoObject( repo )
		, mDiffList( difflist )
	{
		Q_ASSERT( difflist );
	}

	DiffListPrivate::~DiffListPrivate()
	{
		Q_ASSERT( mRepo );

		if( mDiffList )
		{
			git_diff_list_free( mDiffList );
		}
	}

	DiffList::DiffList()
	{
	}

	DiffList::DiffList( DiffListPrivate* _d )
		: d( _d )
	{
	}

	DiffList::DiffList( const DiffList& o )
		: d( o.d )
	{
	}

	DiffList::~DiffList()
	{
	}

	DiffList& DiffList::operator=( const DiffList& other )
	{
		d = other.d;
		return *this;
	}

	bool DiffList::isValid() const
	{
		return d;
	}

	Repository DiffList::repository() const
	{
		return Repository( d ? d->repo() : NULL );
	}

	bool DiffList::mergeOnto( DiffList onto ) const
	{
		if( !d || !onto.isValid() )
		{
			return false;
		}

		int rc = git_diff_merge( onto.d->mDiffList, d->mDiffList );
		return d->handleErrors( rc );
	}

	bool DiffList::consumePatch( PatchConsumer* consumer ) const
	{
		if( !consumer )
		{
			return false;
		}
		return false;
	}

	static int changeListCallBack( void* cb_data, git_diff_delta* delta, float )
	{
		ChangeListConsumer* consumer = (ChangeListConsumer*) cb_data;

		if( consumer->raw( QString::fromUtf8( delta->old_file.path ),
						   QString::fromUtf8( delta->new_file.path ),
						   ChangeListConsumer::Type( delta->status ),
						   delta->similarity,
						   delta->binary ) )
		{
			return -1;
		}

		return GIT_OK;
	}

	bool DiffList::consumeChangeList( ChangeListConsumer* consumer ) const
	{
		if( !consumer )
		{
			return false;
		}

		int rc = git_diff_foreach( d->mDiffList, consumer, changeListCallBack, NULL, NULL );

		if( !d->handleErrors( rc ) )
		{
			return false;
		}

		return true;
	}

}
