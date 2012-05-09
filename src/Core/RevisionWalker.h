
#ifndef GIT_REVISION_WALKER_H
#define GIT_REVISION_WALKER_H

#include "Core/Git.h"

namespace Git
{

	class RevisionWalkerPrivate;

	class ObjectId;
	class Reference;

	class RevisionWalker
	{
	public:
		RevisionWalker();
		RevisionWalker( RevisionWalkerPrivate* _d );
		~RevisionWalker();
		RevisionWalker& operator=( const RevisionWalker& other );

	public:
		bool isValid() const;

		void reset();
		void push( const ObjectId& id );
		void push( const Reference& ref );
		void pushRef( const QByteArray& name );
		void pushHead();

		bool next( ObjectId& oidNext );

		void setSorting( bool topological, bool timed );

	private:
		GitPtr< RevisionWalkerPrivate > d;
	};

}

#endif
