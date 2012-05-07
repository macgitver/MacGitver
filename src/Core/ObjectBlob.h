
#ifndef GIT_OBJECT_BLOB_H
#define GIT_OBJECT_BLOB_H

#include "Core/Git.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"

namespace Git
{

	class ObjectBlob : public Object
	{
	public:
		ObjectBlob();
		ObjectBlob( ObjectPrivate* _d );
		ObjectBlob( const ObjectBlob& o );
	};

}

#endif
