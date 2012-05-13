
#ifndef GIT_OBJECT_H
#define GIT_OBJECT_H

#include "Libs/Git/Git.h"
#include "Libs/Git/ObjectId.h"

namespace Git
{

	class ObjectPrivate;

	class ObjectTree;
	class ObjectBlob;
	class ObjectCommit;
	class ObjectTag;

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

		ObjectTree asTree();
		ObjectCommit asCommit();
		ObjectBlob asBlob();
		ObjectTag asTag();

		bool isTree() const;
		bool isTag() const;
		bool isCommit() const;
		bool isBlob() const;

	protected:
		GitPtr< ObjectPrivate > d;
	};

}

#endif
