
#include "MainWindow.h"
#include "Git.h"

extern "C" {

	// libgit2:
#include "git2.h"
}

#define WRAP_REPO(x) (Repository*)(x)
#define UNWRAP_REPO(x) unwrapRepo(x)

namespace Git
{
	static inline git_repository* unwrapRepo( Repository* x )
	{
		// this is just to make the macro type-safe
		return (git_repository*) x;
	}

	static inline void setError( int rc )
	{
		Q_UNUSED( rc );
		QString errText = QString::fromUtf8( git_lasterror() );
		MainWindow::self().addError( errText );
	}

	Repository* createRepository( const QString& path, bool bare )
	{
		git_repository* repo = 0;

		int rc = git_repository_init( &repo, path.toLatin1().constData(), bare );
		if( rc < GIT_SUCCESS )
		{
			setError( rc );
		}

		return WRAP_REPO( repo );
	}

	void closeRepository( Repository* _repo )
	{
		git_repository* repo = UNWRAP_REPO( _repo );

		git_repository_free( repo );
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
