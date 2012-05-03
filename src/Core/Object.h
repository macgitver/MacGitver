
#ifndef GIT_OBJECT_H
#define GIT_OBJECT_H

#include "Core/Git.h"
#include "Core/ObjectId.h"

namespace Git
{

	class ObjectPrivate;

	class Object
	{
	public:
		Object( ObjectPrivate* _d );
		Object( const Object& other );
		Object();
		~Object();
		
	public:
		Object& operator=( const Object& other );
		bool operator==( const Object& other ) const;
		bool isValid() const;

		ObjectType type() const;
		ObjectId id() const;

	protected:
		GitPtr< ObjectPrivate > d;
	};

}

#endif
