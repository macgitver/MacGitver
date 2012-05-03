
#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>

namespace Git
{

	struct Repository;

	void initLibGit();
	void deinitLibGit();

	Repository* createRepository( const QString& path, bool bare );
	Repository* tryOpenRepository( const QString& path );
	void closeRepository( Repository* repo );

}

#endif
