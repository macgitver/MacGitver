
#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>

typedef struct git_repository git_repository;

namespace Git
{

	void initLibGit();
	void deinitLibGit();

	class Repository
	{
	private:
		Repository( git_repository* repo );

	public:
		~Repository();

	public:
		static Repository* create( const QString& path, bool bare );
		static Repository* open( const QString& path );

		bool isBare() const;

	private:
		git_repository*		mRepo;
	};
}

#endif
