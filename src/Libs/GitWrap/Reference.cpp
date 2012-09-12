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
		return d && d->mRef;	// libGit2 allows a Reference to become invalid though it's still
								// internally refCounted;
	}

	bool Reference::destroy()
	{
		if( isValid() )
		{
			int rc = git_reference_delete( d->mRef );
			if( d->handleErrors( rc ) )
			{
				return false;
			}

			d->mRef = NULL;
		}

		return false;
	}

	QString Reference::name() const
	{
		Q_ASSERT( isValid() );
		return QString::fromUtf8( git_reference_name( d->mRef ) );
	}


	Reference::Type Reference::type() const
	{
		Q_ASSERT( isValid() );
		if( git_reference_type( d->mRef ) == GIT_REF_SYMBOLIC )
			return Symbolic;
		else
			return Direct;
	}

	ObjectId Reference::objectId() const
	{
		Q_ASSERT( isValid() && type() == Direct );
		return ObjectId::fromRaw( git_reference_oid( d->mRef )->id );
	}

	QString Reference::target() const
	{
		Q_ASSERT( isValid() && type() == Symbolic );
		return QString::fromUtf8( git_reference_target( d->mRef ) );
	}

	Repository Reference::repository() const
	{
		return Repository( d ? d->repo() : NULL );
	}

}
