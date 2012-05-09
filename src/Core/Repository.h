
#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <QStringList>
#include <QHash>

#include "Core/Git.h"

namespace Git
{

	class Reference;
	class Object;
	class ObjectCommit;
	class ObjectTag;
	class ObjectBlob;
	class ObjectTree;
	class ObjectId;
	class RevisionWalker;

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

		Reference HEAD();

		Object lookup( const ObjectId& id, ObjectType ot = otAny );
		ObjectCommit lookupCommit( const ObjectId& id );
		ObjectTree lookupTree( const ObjectId& id );
		ObjectBlob lookupBlob( const ObjectId& id );
		ObjectTag lookupTag( const ObjectId& id );

		RevisionWalker newWalker();

		void test();

	private:
		GitPtr< RepositoryPrivate > d;
	};

}

#endif
