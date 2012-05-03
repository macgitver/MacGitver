
#ifndef GIT_OBJECT_TREE_H
#define GIT_OBJECT_TREE_H

#include "Core/Git.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"

namespace Git
{

	class ObjectTree : public Object
	{
	public:
		ObjectTree();
		ObjectTree( ObjectPrivate* _d );
		ObjectTree( const ObjectTree& o );
	
	public:
		ObjectTree subPath( const QByteArray& pathName ) const;
	};

}

#endif
