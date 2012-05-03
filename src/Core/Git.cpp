
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
	{
	}

	Repository::~Repository()
	{
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


	void initLibGit()
	{
		git_threads_init();
	}

	void deinitLibGit()
	{
		git_threads_shutdown();
	}

}
