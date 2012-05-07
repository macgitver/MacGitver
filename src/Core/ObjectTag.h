
#ifndef GIT_OBJECT_TAG_H
#define GIT_OBJECT_TAG_H

#include "Core/Git.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"

namespace Git
{

	class ObjectTag : public Object
	{
	public:
		ObjectTag();
		ObjectTag( ObjectPrivate* _d );
		ObjectTag( const ObjectTag& o );
	};

}

#endif
