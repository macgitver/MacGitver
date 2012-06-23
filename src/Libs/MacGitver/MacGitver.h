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

#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QApplication>

#include "GitWrap/Repository.h"
#include "Heaven/View.h"
#include "MacGitver/MacGitverApi.h"

class Module;
class Modules;
class FSWatcher;
class UserLevelDefinition;
class IMainWindow;


class MGV_CORE_API MacGitver : public QApplication
{
	Q_OBJECT
public:
	MacGitver( int argc, char **argv );
	~MacGitver();

public:
	static MacGitver& self();

public:
	void setRepository( const Git::Repository &repo );
	Git::Repository repository() const;
	Modules* modules();
	FSWatcher* watcher();

	IMainWindow* mainWindow();

	void integrateView( HeavenView* view, Heaven::Positions place );

signals:
	void repositoryChanged( const Git::Repository& repo );

protected:
	void setMainWindow( IMainWindow* mainWindow );

private:
	static MacGitver*	sSelf;

	Modules*			mModules;
	IMainWindow*		mMainWindow;
	FSWatcher*			mWatcher;
	Git::Repository		mRepository;
};

#endif

