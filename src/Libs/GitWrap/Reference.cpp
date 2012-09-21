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
#include "ReferencePrivate.h"
#include "ObjectId.h"
#include "Repository.h"
#include "Reference.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	ReferencePrivate::ReferencePrivate( RepositoryPrivate* repo, git_reference* ref )
		: RepoObject( repo )
		, mRef( ref )
	{
		Q_ASSERT( ref );
	}

	ReferencePrivate::~ReferencePrivate()
	{
		if( mRef )
		{
			git_reference_free( mRef );
		}
	}

	END_INTERNAL_IMPL()

	Reference::Reference()
	{
	}

	Reference::Reference( Internal::ReferencePrivate* _d )
		: d( _d )
	{
	}

	Reference::Reference( const Reference& other )
		: d( other.d )
	{
	}

	Reference::~Reference()
	{
	}


	Reference& Reference::operator=( const Reference& other )
	{
		d = other.d;
		return * this;
	}

	bool Reference::isValid() const
	{
		// libGit2 allows a Reference to become invalid though _WE_ still refCount it...
		return d && d->mRef;
	}

	bool Reference::destroy( Result& result )
	{
		if( !result )
		{
			return false;
		}

		if( !isValid() )
		{
			return false;
		}

		result = git_reference_delete( d->mRef );
		if( !result )
		{
			return false;
		}

		// Clear our ref out. This ReferencePrivate object is invalid from now on.
		// libgit2 free'ed d->mRef.
		d->mRef = NULL;

		return false;
	}

	QString Reference::name() const
	{
		if( !isValid() )
		{
			GitWrap::lastResult().setInvalidObject();
			return QString();
		}

		return QString::fromUtf8( git_reference_name( d->mRef ) );
	}


	Reference::Type Reference::type( Result& result ) const
	{
		if( !result )
		{
			return Invalid;
		}

		if( !isValid() )
		{
			result.setInvalidObject();
			return Invalid;
		}

		if( git_reference_type( d->mRef ) == GIT_REF_SYMBOLIC )
			return Symbolic;
		else
			return Direct;
	}

	ObjectId Reference::objectId( Result& result ) const
	{
		if( !result )
		{
			return ObjectId();
		}

		if( !type( result ) == Direct )	// Does the isValid() check for us, no need to repeat it
		{
			return ObjectId();
		}

		return ObjectId::fromRaw( git_reference_oid( d->mRef )->id );
	}

	QString Reference::target( Result& result ) const
	{
		if( !result )
		{
			return QString();
		}

		if( !type( result ) == Symbolic )	// Does the isValid() check for us, no need to repeat it
		{
			return QString();
		}
		return QString::fromUtf8( git_reference_target( d->mRef ) );
	}

	Repository Reference::repository( Result& result ) const
	{
		if( !result )
		{
			return Repository();
		}

		if( !d )
		{
			return Repository();
		}

		return Repository( d->repo() );
	}

	Reference Reference::resolved( Result& result )
	{
		if( !result )
		{
			return Reference();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Reference();
		}

		git_reference* ref;
		result = git_reference_resolve( &ref, d->mRef );
		if( !result )
		{
			return Reference();
		}

		return new Internal::ReferencePrivate( d->repo(), ref );
	}

	ObjectId Reference::resolveToObjectId( Result& result )
	{
		Reference resolvedRef = resolved( result );
		if( !result )
		{
			return ObjectId();
		}
		return resolvedRef.objectId( result );
	}

}
