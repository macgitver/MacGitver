
#include <QPlastiqueStyle>

#include "Libs/Git/Git.h"

#include "Libs/Core/MacGitver.h"
#include "Libs/Core/Modules.h"
#include "Libs/Core/MainWindow.h"

MacGitver::MacGitver( int argc, char** argv )
	: QApplication( argc, argv )
{
	Git::initLibGit();

	setOrganizationName( "SaCu" );
	setApplicationName( "MacGitver" );
	setStyle( new QPlastiqueStyle );

	mModules = new Modules( this );

	Q_ASSERT( sSelf == NULL );
	sSelf = this;

	MainWindow* mw = new MainWindow;
	mw->show();
}

MacGitver::~MacGitver()
{
	closeRepository();

	delete mModules;

	sSelf = NULL;
}

Modules* MacGitver::modules()
{
	return mModules;
}

Git::Repository MacGitver::repository() const
{
	return mRepository;
}

void MacGitver::closeRepository()
{
	if( mRepository.isValid() )
	{
		setRepository( Git::Repository() );
	}
}

void MacGitver::openedRepository( const Git::Repository& repo )
{
	setRepository( repo );
}

void MacGitver::setRepository( const Git::Repository& repo )
{
	mRepository = repo;

	mModules->repositoryChanged( repo );

	emit repositoryChanged( mRepository );
}

MacGitver* MacGitver::sSelf = NULL;

MacGitver& MacGitver::self()
{
	Q_ASSERT( sSelf );
	return *sSelf;
}
