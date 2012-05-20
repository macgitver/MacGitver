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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

#include "Libs/Git/Git.h"
#include "Libs/Git/Repository.h"

#include "Libs/Heaven/HTopLevelWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

private slots:
	void onRepositoryCreate();
	void onRepositoryOpen();

private:
	void setupUi();

public slots:
	void repositoryChanged( const Git::Repository& repo );

public:
	void integrateView( HeavenView* view, Heaven::Positions position );
	void addError( const QString& err );

private:
	QMenu*					mmuRepository;
	QAction*				macRepositoryOpen;
	QAction*				macRepositoryClose;
	QAction*				macRepositoryCreate;
	QAction*				macRepositoryClone;
	QAction*				macRepositoryOptions;
	QAction*				macRepositoryQuit;
	QMenu*					mmuWorkingTree;

	QLabel*					mLblCurrentBranch;
	HeavenTopLevelWidget*	mTop;

	Git::Repository			mRepo;
};

#endif
