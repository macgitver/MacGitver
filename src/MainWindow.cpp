
#include <QFileDialog>
#include <QApplication>
#include <QMenuBar>

#include "MainWindow.h"
#include "Repo/CreateRepositoryDlg.h"

MainWindow* MainWindow::sSelf = NULL;

MainWindow::MainWindow()
	: mRepo()
{
	sSelf = this;
	setupUi();
}

MainWindow::~MainWindow()
{
	closeRepository();
}

void MainWindow::onRepositoryCreate()
{
	CreateRepositoryDlg().exec();
}

void MainWindow::onRepositoryOpen()
{
	QString fn = QFileDialog::getExistingDirectory( this, trUtf8( "Open repository" ) );
	if( fn.isEmpty() )
	{
		return;
	}

	Git::Repository repo = Git::Repository::open( fn );
	if( repo.isValid() )
	{
		switchToRepo( repo );
	}
}

void MainWindow::setupUi()
{
	mmuRepository			= menuBar()->addMenu( trUtf8( "&Repository" ) );
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "&Open..." ),
														this, SLOT(onRepositoryOpen()) );
							  mmuRepository->addSeparator();
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "Create &new..." ),
														this, SLOT(onRepositoryCreate()) );
							  mmuRepository->addSeparator();
	macRepositoryQuit		= mmuRepository->addAction( trUtf8( "&Quit" ),
														qApp, SLOT(quit()) );

	mmuWorkingTree			= menuBar()->addMenu( trUtf8( "&Working Tree" ) );
	//mmuWorkingTree->addAction()

	setWindowTitle( trUtf8( "MacGitver" ) );
}

void MainWindow::addError( const QString& err )
{
	qDebug( "Error occured:\n%s", qPrintable( err ) );
}

void MainWindow::closeRepository()
{
	if( !mRepo.isValid() )
	{
		return;
	}

	emit repositoryChanged( Git::Repository() );

	mRepo = Git::Repository();
}

void MainWindow::switchToRepo( const Git::Repository& repo )
{
	closeRepository();

	mRepo = repo;
	emit repositoryChanged( mRepo );
}
