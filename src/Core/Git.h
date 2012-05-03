
#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>

namespace Git
{

	void initLibGit();
	void deinitLibGit();

	class Repository;
	class Index;

	template< class T >
	class GitPtr
	{
	public:
		GitPtr();
		GitPtr( const GitPtr< T >& o );
		GitPtr( T* o );
		~GitPtr();

		GitPtr< T >& operator=( const GitPtr< T >& o );
		bool operator==( const GitPtr< T >& o ) const;
		bool operator==( T* o ) const;

		T* operator->();
		const T* operator->() const;

		T* operator*();
		const T* operator*() const;

		operator bool() const;
		operator T*();
		operator const T*() const;

	private:
		T* d;
	};

	enum ObjectType
	{
		otTree,
		otCommit,
		otBlob,
		otTag,

		otAny = -1
	};

}

#endif
