
#ifndef GIT_P_H
#define GIT_P_H

#include "git2.h"

#include "Core/Git.h"

namespace Git
{

	class RepositoryPrivate : public QSharedData
	{
	public:
		RepositoryPrivate( git_repository* repo );

	public:
		git_repository*		mRepo;
		Index				mIndex;
	};

	class IndexPrivate : public QSharedData
	{
	public:
		IndexPrivate( RepositoryPrivate* repo, git_index* index );

	private:
		RepositoryPrivate*	mRepo;
		git_index*			mIndex;
	};


}

#endif
