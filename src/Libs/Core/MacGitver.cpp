
#include <QPlastiqueStyle>

#include "Libs/Git/Git.h"

#include "Libs/Core/MacGitver.h"
#include "Libs/Core/MainWindow.h"

MacGitver::MacGitver( int argc, char** argv )
	: QApplication( argc, argv )
{
	Q_ASSERT( sSelf == NULL );
	sSelf = this;

	Git::initLibGit();

	setOrganizationName( "SaCu" );
	setApplicationName( "MacGitver" );
	setStyle( new QPlastiqueStyle );

	MainWindow* mw = new MainWindow;
	mw->show();
}

MacGitver::~MacGitver()
{
	sSelf = NULL;
}

MacGitver* MacGitver::sSelf = NULL;

MacGitver& MacGitver::self()
{
	Q_ASSERT( sSelf );
	return *sSelf;
}
