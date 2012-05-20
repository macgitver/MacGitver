/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QStatusBar>
#include <QFileDialog>
#include <QApplication>
#include <QMenuBar>
#include <QDockWidget>

#include "GitWrap/ObjectId.h"
#include "GitWrap/Reference.h"

#include "MacGitver/MacGitver.h"

#include "Main/MainWindow.h"
#include "Main/ConfigDialog.h"

#include "Dlgs/Repository/CreateRepositoryDlg.h"

#include "Heaven/TopLevelWidget.h"

#include "Views/Diff/DiffView.h"
#include "Views/WorkingTree/IndexWidget.h"

MainWindow::MainWindow()
	: mRepo()
{
	setupUi();

	connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
			 SLOT(repositoryChanged(Git::Repository)) );
}

MainWindow::~MainWindow()
{
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
		MacGitver::self().setRepository( repo );
	}
}

void MainWindow::setupUi()
{
	QFile styleFile( ":/MacGitver.qss" );
	styleFile.open( QFile::ReadOnly );
	setStyleSheet( styleFile.readAll() );

	mmuRepository			= menuBar()->addMenu( trUtf8( "&Repository" ) );
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "&Open..." ),
														this, SLOT(onRepositoryOpen()) );
							  mmuRepository->addSeparator();
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "Create &new..." ),
														this, SLOT(onRepositoryCreate()) );
							  mmuRepository->addSeparator();
	macRepositoryOptions	= mmuRepository->addAction( trUtf8( "&Preferences..." ),
														this, SLOT(onPreferences()) );
							  mmuRepository->addSeparator();
	macRepositoryQuit		= mmuRepository->addAction( trUtf8( "&Quit" ),
														qApp, SLOT(quit()) );

	mmuWorkingTree			= menuBar()->addMenu( trUtf8( "&Working Tree" ) );
	//mmuWorkingTree->addAction()

	setWindowTitle( trUtf8( "MacGitver" ) );

	statusBar()->addPermanentWidget( mLblCurrentBranch = new QLabel() );

	mTop = new HeavenTopLevelWidget();
	setCentralWidget( mTop );

	IndexWidget* iw = new IndexWidget;
	mTop->addView( iw, Heaven::Central );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		iw, SLOT(repositoryChanged(Git::Repository)) );

	mTop->addView( new DiffView, Heaven::Central );
}

void MainWindow::repositoryChanged( const Git::Repository& repo )
{
	mRepo = repo;

	if( mRepo.isValid() )
	{
		Git::Reference HEAD = mRepo.HEAD();
		QString curBranch;
		if( HEAD.name() != "HEAD" )
		{
			curBranch = trUtf8( "on branch <b>%1</b>" ).arg( HEAD.name().mid( 11 ).constData() );
		}
		else
		{
			curBranch = trUtf8( "HEAD detached: <b>%1</b>" ).arg( HEAD.objectId().toString() );
		}

		mLblCurrentBranch->setText( curBranch );
	}
	else
	{
		mLblCurrentBranch->setText( QString() );
	}
}

void MainWindow::integrateView( HeavenView* view, Heaven::Positions position )
{
	mTop->addView( view, position );
}

void MainWindow::onPreferences()
{
	ConfigDialog d;
//	new GeneralConfigPage( &d );
//	MacGitver::self().modules()->setupConfigPages( &d );
	d.exec();
}
