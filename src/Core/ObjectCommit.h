
#ifndef GIT_OBJECT_COMMIT_H
#define GIT_OBJECT_COMMIT_H

#include "Core/Git.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"
#include "Core/Signature.h"

namespace Git
{

	class ObjectTree;

	class ObjectCommit : public Object
	{
	public:
		ObjectCommit();
		ObjectCommit( ObjectPrivate* _d );
		ObjectCommit( const ObjectCommit& o );

	public:
		ObjectTree tree();
		ObjectId treeId();

		QList< ObjectId > parentCommitIds();
		QList< ObjectCommit > parentCommits();
		unsigned int numParentCommits();

		Signature author();
		Signature committer();
	};

}

#endif
