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
#include "Object.h"
#include "ObjectTree.h"
#include "ObjectTag.h"
#include "ObjectCommit.h"
#include "ObjectBlob.h"

namespace Git
{
	ObjectPrivate::ObjectPrivate( RepositoryPrivate* repo, git_object* o )
		: mRepo( repo )
		, mObj( o )
	{
		mRepo->ref();
	}

	ObjectPrivate::~ObjectPrivate()
	{
		git_object_free( mObj );
		mRepo->deref();
	}

	Object::Object( ObjectPrivate* _d )
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

	ObjectType Object::type() const
	{
		Q_ASSERT( d );
		switch( git_object_type( d->mObj ) )
		{
		case GIT_OBJ_BLOB:		return otBlob;
		case GIT_OBJ_TREE:		return otTree;
		case GIT_OBJ_COMMIT:	return otCommit;
		case GIT_OBJ_TAG:		return otTag;

		default:				return otAny;
		}
	}

	ObjectId Object::id() const
	{
		Q_ASSERT( d );
		const git_oid* oid = git_object_id( d->mObj );
		return ObjectId::fromRaw( oid->id );
	}


	ObjectTree Object::asTree()
	{
		ObjectTree o;
		if( isTree() )
		{
			o = ObjectTree( d );
		}
		return o;
	}

	ObjectCommit Object::asCommit()
	{
		ObjectCommit o;
		if( isCommit() )
		{
			o = ObjectCommit( d );
		}
		return o;
	}

	ObjectBlob Object::asBlob()
	{
		ObjectBlob o;
		if( isBlob() )
		{
			o = ObjectBlob( d );
		}
		return o;
	}

	ObjectTag Object::asTag()
	{
		ObjectTag o;
		if( isTag() )
		{
			o = ObjectTag( d );
		}
		return o;
	}

	bool Object::isTree() const
	{
		return type() == otTree;
	}

	bool Object::isTag() const
	{
		return type() == otTag;
	}

	bool Object::isCommit() const
	{
		return type() == otCommit;
	}

	bool Object::isBlob() const
	{
		return type() == otBlob;
	}

}
