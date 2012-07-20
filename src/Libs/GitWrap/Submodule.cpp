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

#include "Submodule.h"
#include "ObjectId.h"
#include "Repository.h"
#include "RepositoryPrivate.h"

namespace Git
{

	static inline git_submodule* getSM( const RepositoryPrivate* repo, const QString& smName )
	{
		git_submodule* result = NULL;

		int rc = git_submodule_lookup( &result, repo->mRepo, smName.toUtf8().constData() );
		if( rc < 0 )
		{
			return NULL;
		}

		return result;
	}

	Submodule::Submodule()
		: mRepo( NULL )
	{
	}

	Submodule::Submodule( RepositoryPrivate* repo, const QString& name )
		: mRepo( repo )
		, mName( name )
	{
		isValid();	// do a lookup, so we know we're valid at least once
	}

	Submodule::~Submodule()
	{
	}

	bool Submodule::isValid()
	{
		if( !mRepo || mName.isEmpty() )
		{
			return false;
		}

		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			mRepo = NULL;
			mName = QString();
		}

		return mRepo;
	}

	QString Submodule::name() const
	{
		return mName;
	}

	QString Submodule::path() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		const char* data = sm ? sm->path : NULL;
		return data ? QString::fromUtf8( data ) : QString();
	}

	QString Submodule::url() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		const char* data = sm ? sm->url : NULL;
		return data ? QString::fromUtf8( data ) : QString();
	}

	bool Submodule::fetchRecursive() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		return sm ? sm->fetch_recurse : false;
	}

	Submodule::IgnoreStrategy Submodule::ignoreStrategy() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return None;
		}
		switch( sm->ignore )
		{
		case GIT_SUBMODULE_IGNORE_ALL:			return All;
		case GIT_SUBMODULE_IGNORE_DIRTY:		return Dirty;
		case GIT_SUBMODULE_IGNORE_UNTRACKED:	return Untracked;
		case GIT_SUBMODULE_IGNORE_NONE:
		default:								return None;
		}
	}

	Submodule::UpdateStrategy Submodule::updateStrategy() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return Ignore;
		}
		switch( sm->update )
		{
		default:
		case GIT_SUBMODULE_UPDATE_CHECKOUT:	return Checkout;
		case GIT_SUBMODULE_UPDATE_MERGE:	return Merge;
		case GIT_SUBMODULE_UPDATE_REBASE:	return Rebase;
			// libgit2 doesn't support None
		}
	}

	ObjectId Submodule::currentSHA1() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return ObjectId();
		}

		return ObjectId::fromRaw( sm->oid.id );
	}

}
