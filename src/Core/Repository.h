
#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <QStringList>
#include <QHash>

#include "Core/Git.h"

namespace Git
{

	class RepositoryPrivate;

	typedef QHash< QString, int > StatusHash;

	class Repository
	{
	public:
		explicit Repository( RepositoryPrivate* _d );
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

		QString basePath() const;

		QStringList allReferences();
		QStringList allBranches();
		QStringList branches( bool local = true, bool remote = false );
		QStringList allTags();

		Index index();

		StatusHash statusHash();

		void test();

	private:
		GitPtr< RepositoryPrivate > d;
	};

}

#endif
