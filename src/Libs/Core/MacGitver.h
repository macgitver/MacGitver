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

#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QApplication>

#include "GitWrap/Repository.h"

class Module;
class Modules;
class MainWindow;

class MacGitver : public QApplication
{
	Q_OBJECT
public:
	MacGitver( int argc, char **argv );
	~MacGitver();

public:
	static MacGitver& self();

public:
	void closeRepository();
	void openedRepository( const Git::Repository &repo );
	Git::Repository repository() const;
	Modules* modules();

	MainWindow* mainWindow();

signals:
	void repositoryChanged( const Git::Repository& repo );

private:
	void setRepository( const Git::Repository &repo );

private:
	static MacGitver*	sSelf;

	Modules*			mModules;
	MainWindow*			mMainWindow;
	Git::Repository		mRepository;
};

#endif

