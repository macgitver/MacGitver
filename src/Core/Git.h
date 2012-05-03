
#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>

#include <QSharedData>

namespace Git
{

	void initLibGit();
	void deinitLibGit();

	class Repository;
	class Index;

	class RepositoryPrivate;

	class Repository
	{
	private:
		Repository( RepositoryPrivate* d );
	public:
		Repository();
		Repository( const Repository& other );

	public:
		~Repository();

	public:
		static Repository create( const QString& path, bool bare );
		static Repository open( const QString& path );

		bool isBare() const;

		Index index();

	private:
		QExplicitlySharedDataPointer< RepositoryPrivate > d;
	};

	class IndexEntry
	{

	private:
		git_index*			mIndex;
	};

	class Index
	{
		friend class Repository;
	private:
		Index( git_index* index );

	public:
		~Index();

	public:
		int count() const;
		IndexEntry* at( int index );

	private:
		QExplicitlySharedDataPointer< IndexPrivate > d;
	};
}

#endif
