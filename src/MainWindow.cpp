
#include <QApplication>
#include <QMenuBar>

#include "MainWindow.h"
#include "Repo/CreateRepositoryDlg.h"

MainWindow* MainWindow::sSelf = NULL;

MainWindow::MainWindow()
{
	sSelf = this;
	setupUi();
}

void MainWindow::onRepositoryCreate()
{
	CreateRepositoryDlg().exec();
}

void MainWindow::setupUi()
{
	mmuRepository			= menuBar()->addMenu( trUtf8( "&Repository" ) );
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "Create &new..." ),
														this, SLOT(onRepositoryCreate()) );
	macRepositoryQuit		= mmuRepository->addAction( trUtf8( "&Quit" ),
														qApp, SLOT(quit()) );

	setWindowTitle( trUtf8( "MacGitver" ) );
}

void MainWindow::addError( const QString& err )
{
	qDebug( "Error occured:\n%s", qPrintable( err ) );
}
