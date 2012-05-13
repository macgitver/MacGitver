
#ifndef GIT_OBJECT_TAG_H
#define GIT_OBJECT_TAG_H

#include "Libs/Git/Git.h"
#include "Libs/Git/ObjectId.h"
#include "Libs/Git/Object.h"

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
