
#include <QPlastiqueStyle>
#include <QApplication>

#include "Core/Git.h"

#include "MainWindow.h"

int main( int c, char** v )
{
	Git::initLibGit();

	QApplication::setOrganizationName( "SaCu" );
	QApplication::setApplicationName( "MacGitver" );

	QApplication a( c, v );
	a.setStyle( new QPlastiqueStyle );

	MainWindow* mw = new MainWindow;
	mw->show();

	return a.exec();
}
