/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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
#include <QFile>
#include <QApplication>
#include <QMenuBar>
#include <QDockWidget>

#include "GitWrap/ObjectId.h"
#include "GitWrap/Reference.h"

#include "MacGitver/MacGitver.h"
#include "MacGitver/Modules.h"

#include "Main/MainWindow.h"
#include "Main/ConfigDialog.h"
#include "Main/GeneralConfigPage.h"

#include "Heaven/TopLevelWidget.h"

#include "Views/Diff/DiffView.h"

#include "ui_AboutDlg.h"

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

void MainWindow::setupUi()
{
	QFile styleFile( ":/MacGitver.qss" );
	styleFile.open( QFile::ReadOnly );
	setStyleSheet( styleFile.readAll() );

	setupActions( this );

	setMenuBar( mbMainMenuBar->menuBarFor( this ) );

	setWindowTitle( trUtf8( "MacGitver" ) );

	statusBar()->addPermanentWidget( mLblCurrentBranch = new QLabel() );

	mTop = new HeavenTopLevelWidget();
	setCentralWidget( mTop );

	mTop->addView( new DiffView, Heaven::Central );

	addToolBar( tbMainBar->toolBarFor( this ) );
}

void MainWindow::repositoryChanged( const Git::Repository& repo )
{
	mRepo = repo;
	setHeadLabel();
}

void MainWindow::setHeadLabel()
{
	if( mRepo.isValid() )
	{
		QString curBranch;
		Git::Reference HEAD = mRepo.HEAD();

		if( HEAD.isValid() )
		{
			if( HEAD.name() != "HEAD" )
			{
				curBranch = trUtf8( "on branch <b>%1</b>" ).arg( HEAD.name().mid( 11 ).constData() );
			}
			else
			{
				curBranch = trUtf8( "on detached HEAD at <b>%1</b>" ).arg( HEAD.objectId().toString() );
			}
		}
		else
		{
			curBranch = trUtf8( "<b style=\"color: red;\">Repository has no HEAD (yet)?</b>" );
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

QWidget* MainWindow::widget()
{
	return this;
}

void MainWindow::onHelpAbout()
{
	QDialog d;
	Ui::AboutDlg u;
	u.setupUi( &d );
	d.exec();
}

void MainWindow::onToolsPreferences()
{
	ConfigDialog d;
	d.addPage( new GeneralConfigPage( &d ) );
	MacGitver::self().modules()->setupConfigPages( &d );
	d.exec();
}
