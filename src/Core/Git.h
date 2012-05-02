
#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>

namespace Git
{

	struct Repository;

	Repository* createRepository( const QString& path, bool bare );

}

#endif
