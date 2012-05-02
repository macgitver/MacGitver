
#include <QApplication>

#include "MainWindow.h"

int main( int c, char** v )
{
	QApplication::setOrganizationName( "SaCu" );
	QApplication::setApplicationName( "MacGitver" );

	QApplication a( c, v );

	MainWindow* mw = new MainWindow;
	mw->show();

	return a.exec();
}
