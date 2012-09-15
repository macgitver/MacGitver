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
#include "ObjectId.h"
#include "Object.h"
#include "ObjectTree.h"
#include "ObjectTag.h"
#include "ObjectCommit.h"
#include "ObjectBlob.h"
#include "ObjectPrivate.h"
#include "Repository.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	ObjectPrivate::ObjectPrivate( RepositoryPrivate* repo, git_object* o )
		: RepoObject( repo )
		, mObj( o )
	{
		Q_ASSERT( o );
	}

	ObjectPrivate::~ObjectPrivate()
	{
		git_object_free( mObj );
	}

	END_INTERNAL_IMPL()

	Object::Object( Internal::ObjectPrivate* _d )
		: d( _d )
	{
	}

	Object::Object( const Object& other )
		: d( other.d )
	{
	}

	Object::Object()
	{
	}

	Object::~Object()
	{
	}

	Object& Object::operator=( const Object& other )
	{
		d = other.d;
		return * this;
	}

	bool Object::operator==( const Object& other ) const
	{
		return d == other.d;
	}

	bool Object::isValid() const
	{
		return d;
	}

	ObjectType Object::type( Result& result ) const
	{
		if( !result )
		{
			return otAny;
		}

		if( !d )
		{
			result.setInvalidObject();
			return otAny;
		}

		switch( git_object_type( d->mObj ) )
		{
		case GIT_OBJ_BLOB:		return otBlob;
		case GIT_OBJ_TREE:		return otTree;
		case GIT_OBJ_COMMIT:	return otCommit;
		case GIT_OBJ_TAG:		return otTag;

		default:				return otAny;
		}
	}

	ObjectId Object::id( Result& result ) const
	{
		if( !result )
		{
			return ObjectId();
		}

		if( !d )
		{
			result.setInvalidObject();
			return ObjectId();
		}

		const git_oid* oid = git_object_id( d->mObj );
		return ObjectId::fromRaw( oid->id );
	}


	ObjectTree Object::asTree( Result& result )
	{
		ObjectTree o;
		if( isTree( result ) )
		{
			o = ObjectTree( d );
		}
		return o;
	}

	ObjectCommit Object::asCommit( Result& result )
	{
		ObjectCommit o;
		if( isCommit( result ) )
		{
			o = ObjectCommit( d );
		}
		return o;
	}

	ObjectBlob Object::asBlob( Result& result )
	{
		ObjectBlob o;
		if( isBlob( result ) )
		{
			o = ObjectBlob( d );
		}
		return o;
	}

	ObjectTag Object::asTag( Result& result )
	{
		ObjectTag o;
		if( isTag( result ) )
		{
			o = ObjectTag( d );
		}
		return o;
	}

	bool Object::isTree( Result& result ) const
	{
		return type( result ) == otTree;
	}

	bool Object::isTag( Result& result ) const
	{
		return type( result ) == otTag;
	}

	bool Object::isCommit( Result& result ) const
	{
		return type( result ) == otCommit;
	}

	bool Object::isBlob( Result& result ) const
	{
		return type( result ) == otBlob;
	}

	Repository Object::repository( Result& result ) const
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

}
