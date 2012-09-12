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

	static inline git_submodule* getSM( const Internal::RepositoryPrivate* repo, const QString& smName )
	{
		git_submodule* result = NULL;

		if( repo && repo->mRepo )
		{
			int rc = git_submodule_lookup( &result, repo->mRepo, smName.toUtf8().constData() );
			if( rc < 0 )
			{
				return NULL;
			}
		}

		return result;
	}

	Submodule::Submodule()
		: mRepo( NULL )
	{
	}

	Submodule::Submodule( Internal::RepositoryPrivate* repo, const QString& name )
		: mRepo( repo )
		, mName( name )
	{
		isValid();	// do a lookup, so we know we're valid at least once
	}

	Submodule::Submodule( const Submodule& other )
	{
		mRepo = other.mRepo;
		mName = other.mName;
	}

	Submodule& Submodule::operator=( const Submodule& other )
	{
		mRepo = other.mRepo;
		mName = other.mName;
		return *this;
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
		if( !sm )
		{
			return QString();
		}
		const char* data = git_submodule_path( sm );
		return data ? QString::fromUtf8( data ) : QString();
	}

	QString Submodule::url() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return QString();
		}
		const char* data = git_submodule_url( sm );
		return data ? QString::fromUtf8( data ) : QString();
	}

	bool Submodule::fetchRecursive() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return git_submodule_fetch_recurse_submodules( sm );
		}
		return false;
	}

	Submodule::IgnoreStrategy Submodule::ignoreStrategy() const
	{
		git_submodule* sm = getSM( mRepo, mName );
		if( !sm )
		{
			return None;
		}
		switch( git_submodule_ignore( sm ) )
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
		switch( git_submodule_update( sm ) )
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

		const git_oid* oid = git_submodule_head_oid( sm );
		if( !oid )
		{
			return ObjectId();
		}

		return ObjectId::fromRaw( oid->id );
	}

}
