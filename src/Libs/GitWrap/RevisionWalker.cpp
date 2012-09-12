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

#include "RevisionWalker.h"
#include "RevisionWalkerPrivate.h"
#include "ObjectId.h"
#include "Reference.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	RevisionWalkerPrivate::RevisionWalkerPrivate( RepositoryPrivate* repo, git_revwalk* walker )
		: RepoObject( repo )
		, mWalker( walker )
	{
		Q_ASSERT( walker );
	}

	RevisionWalkerPrivate::~RevisionWalkerPrivate()
	{
		git_revwalk_free( mWalker );
	}

	END_INTERNAL_IMPL()

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
			int rc = git_revwalk_push( d->mWalker, (const git_oid*) id.raw() );
			d->handleErrors( rc );
		}
	}

	void RevisionWalker::push( const Reference& ref )
	{
		pushRef( ref.name() );
	}

	void RevisionWalker::pushRef( const QString& name )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_push_ref( d->mWalker, name.toUtf8().constData() );
			d->handleErrors( rc );
		}
	}

	void RevisionWalker::pushHead()
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_push_head( d->mWalker );
			d->handleErrors( rc );
		}
	}

	void RevisionWalker::hide( const ObjectId& id )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_hide( d->mWalker, (const git_oid*) id.raw() );
			d->handleErrors( rc );
		}
	}

	void RevisionWalker::hide( const Reference& ref )
	{
		hideRef( ref.name() );
	}

	void RevisionWalker::hideRef( const QString& name )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_hide_ref( d->mWalker, name.toUtf8().constData() );
			d->handleErrors( rc );
		}
	}

	void RevisionWalker::hideHead()
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_revwalk_hide_head( d->mWalker );
			d->handleErrors( rc );
		}
	}

	bool RevisionWalker::next( ObjectId& oidNext )
	{
		Q_ASSERT( d );

		if( d )
		{
			git_oid oid;
			int rc = git_revwalk_next( &oid, d->mWalker );
			if( rc == GIT_ITEROVER )
			{
				return false;
			}

			if( !d->handleErrors( rc ) )
			{
				return false;
			}

			oidNext = ObjectId::fromRaw( oid.id );
			return true;
		}

		return false;
	}

	QVector< ObjectId > RevisionWalker::all()
	{
		QVector< ObjectId > ids ;
		ObjectId id;

		while( next( id ) )
		{
			ids.append( id );
		}

		return ids;
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

