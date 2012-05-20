
#include "Main/MacGitverMain.h"
#include "Main/MainWindow.h"

MacGitverMain::MacGitverMain( int argc, char** argv )
	: MacGitver( argc, argv )
{
}

int MacGitverMain::exec()
{
	MainWindow* mw = new MainWindow;
	setMainWindow( mw );

	QMetaObject::invokeMethod( mw, "show", Qt::QueuedConnection );

	return MacGitver::exec();
}
