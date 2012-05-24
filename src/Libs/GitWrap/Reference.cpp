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
#include "ObjectId.h"
#include "Reference.h"

namespace Git
{

	ReferencePrivate::ReferencePrivate( RepositoryPrivate* repo, git_reference* ref )
		: mRepo( repo )
		, mRef( ref )
	{
		Q_ASSERT( mRepo );
		mRepo->ref();
	}

	ReferencePrivate::~ReferencePrivate()
	{
		if( mRef )
		{
			git_reference_free( mRef );
		}

		mRepo->deref();
	}

	Reference::Reference()
	{
	}

	Reference::Reference( ReferencePrivate* _d )
		: d( _d )
	{
	}

	bool Reference::isValid() const
	{
		return d && d->mRef;	// libGit2 allows a Reference to become invalid though it's still
								// internally refCounted;
	}

	void Reference::destroy()
	{
		if( isValid() )
		{
			int rc = git_reference_delete( d->mRef );
			if( rc == GIT_OK )
			{
				d->mRef = NULL;
			}
		}
	}

	QByteArray Reference::name() const
	{
		Q_ASSERT( isValid() );
		return QByteArray( git_reference_name( d->mRef ) );
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

	QByteArray Reference::target() const
	{
		Q_ASSERT( isValid() && type() == Symbolic );
		return QByteArray( git_reference_target( d->mRef ) );
	}

}
