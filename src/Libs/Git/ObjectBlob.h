
#ifndef GIT_OBJECT_BLOB_H
#define GIT_OBJECT_BLOB_H

#include "Libs/Git/Git.h"
#include "Libs/Git/ObjectId.h"
#include "Libs/Git/Object.h"

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
