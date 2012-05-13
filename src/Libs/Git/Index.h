
#ifndef GIT_INDEX_H
#define GIT_INDEX_H

#include "Core/Git.h"

namespace Git
{

	class IndexPrivate;

	class Index
	{
	public:
		explicit Index( IndexPrivate* _d );

	public:
		Index( const Index& other );
		Index();
		~Index();

	public:
		Index& operator=( const Index& other );

	public:
		bool isValid() const;
		int count() const;

		Repository repository() const;

	private:
		GitPtr< IndexPrivate > d;
	};

}

#endif
