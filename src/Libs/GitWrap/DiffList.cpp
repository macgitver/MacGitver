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
#include "PatchConsumer.h"
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

	static int patchFileCallBack( void* cb_data, git_diff_delta* delta, float )
	{
		PatchConsumer* pc = (PatchConsumer*) cb_data;

		if( pc->startFile( QString::fromUtf8( delta->old_file.path ),
						   QString::fromUtf8( delta->new_file.path ),
						   PatchConsumer::Type( delta->status ),
						   delta->similarity,
						   delta->binary ) )
		{
			return GIT_ERROR;
		}

		return GIT_OK;
	}

	static int patchHunkCallBack( void* cb_data, git_diff_delta* delta, git_diff_range* range,
								  const char* header, size_t header_len )
	{
		PatchConsumer* pc = (PatchConsumer*) cb_data;

		if( pc->startHunk( range->new_start, range->new_lines,
						   range->old_start, range->old_lines,
						   header ? QString::fromUtf8( header, int( header_len ) ) : QString() ) )
		{
			return GIT_ERROR;
		}

		return GIT_OK;
	}

	static int patchDataCallBack( void* cb_data, git_diff_delta* delta, git_diff_range* range,
								  char line_origin, const char *content, size_t content_len )
	{
		PatchConsumer* pc = (PatchConsumer*) cb_data;

		QString ct;
		if( content && content_len )
		{
			if( content[ content_len - 1 ] == '\n' )
				--content_len;

			ct = QString::fromUtf8( content, int( content_len ) );
		}

		switch( line_origin )
		{
		case GIT_DIFF_LINE_CONTEXT:
			if( pc->appendContext( ct ) )
			{
				return GIT_ERROR;
			}
			break;

		case GIT_DIFF_LINE_ADDITION:
			if( pc->appendAddition( ct ) )
			{
				return GIT_ERROR;
			}
			break;

		case GIT_DIFF_LINE_DELETION:
			if( pc->appendDeletion( ct ) )
			{
				return GIT_ERROR;
			}
			break;

		default:
			qDebug( "Foo: t=%i", int(line_origin) );

		}

		return GIT_OK;
	}

	bool DiffList::consumePatch( PatchConsumer* consumer ) const
	{
		if( !d || !d->mDiffList )
		{
			return false;
		}

		if( !consumer )
		{
			return false;
		}

		int rc = git_diff_foreach( d->mDiffList, consumer, patchFileCallBack, patchHunkCallBack,
								   patchDataCallBack );

		if( !d->handleErrors( rc ) )
		{
			return false;
		}

		return true;
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
			return GIT_ERROR;
		}

		return GIT_OK;
	}

	bool DiffList::consumeChangeList( ChangeListConsumer* consumer ) const
	{
		if( !d || !d->mDiffList )
		{
			return false;
		}

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
