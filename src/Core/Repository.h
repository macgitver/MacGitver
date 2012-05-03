
#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include "Core/Git.h"

namespace Git
{

	class RepositoryPrivate;

	class Repository
	{
	private:
		Repository( RepositoryPrivate* _d );
	public:
		Repository();
		Repository( const Repository& other );
		Repository& operator=( const Repository& other );

	public:
		~Repository();

	public:
		static Repository create( const QString& path, bool bare );
		static Repository open( const QString& path );

		bool isValid() const;

		bool isBare() const;

		Index index();

	private:
		GitPtr< RepositoryPrivate > d;
	};

}

#endif