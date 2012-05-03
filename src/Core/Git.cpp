
#include "MainWindow.h"
#include "Git.h"

	// libgit2:
#include "git2.h"

namespace Git
{

	static inline void setError( int rc )
	{
		Q_UNUSED( rc );
		QString errText = QString::fromUtf8( git_lasterror() );
		MainWindow::self().addError( errText );
	}

	Repository::Repository( git_repository* repo )
		: mRepo( repo )
		, mIndex( NULL )
	{
	}

	Repository::~Repository()
	{
		if( mIndex )
		{
			delete mIndex;
			mIndex = NULL;
		}
		git_repository_free( mRepo );
	}

	Repository* Repository::create( const QString& path, bool bare )
	{
		git_repository* repo = NULL;

		int rc = git_repository_init( &repo, path.toLatin1().constData(), bare );
		if( rc < GIT_SUCCESS )
		{
			setError( rc );
			return NULL;
		}

		return new Repository( repo );
	}

	Repository* Repository::open( const QString& path )
	{
		git_repository* repo = NULL;

		int rc = git_repository_open( &repo, path.toLatin1().constData() );

		if( rc < GIT_SUCCESS )
		{
			setError( rc );
			return NULL;
		}

		return new Repository( repo );
	}

	bool Repository::isBare() const
	{
		Q_ASSERT( mRepo );
		return git_repository_is_bare( mRepo );
	}

	Index Repository::index()
	{
		if( isBare() )
		{
			return NULL;
		}

		if( !mIndex )
		{
			git_index* index = NULL;

			int rc = git_repository_index( &index, mRepo );

			if( rc < GIT_SUCCESS )
			{
				setError( rc );
				return NULL;
			}

			mIndex = new Index( index );
		}

		return mIndex;
	}

	// INDEX

	Index::Index( git_index* index )
		: mIndex( index )
	{
	}

	Index::~Index()
	{
		if( mIndex )
		{
			git_index_free( mIndex );
		}
	}

	int Index::count() const
	{
		return git_index_entrycount( mIndex );
	}

	IndexEntry* Index::at( unsigned int at )
	{
		return new IndexEntry( git_index_get( mIndex, at ) );
	}

	// tools

	void initLibGit()
	{
		git_threads_init();
	}

	void deinitLibGit()
	{
		git_threads_shutdown();
	}

}
