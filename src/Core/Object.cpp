
#include "Core/Git_p.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"

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

}
