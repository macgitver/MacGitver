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

#include <QPlastiqueStyle>

#include "GitWrap/Git.h"

#include "MacGitver/MacGitver.h"
#include "MacGitver/Modules.h"
#include "MacGitver/FSWatcher.h"

#include "Interfaces/IMainWindow.h"

MacGitver::MacGitver( int argc, char** argv )
	: QApplication( argc, argv )
	, mMainWindow( NULL )
{
	Git::initLibGit();

	setOrganizationName( "SaCu" );
	setApplicationName( "MacGitver" );
	setStyle( new QPlastiqueStyle );

	mWatcher = new FSWatcher( this );

	mModules = new Modules( this );

	Q_ASSERT( sSelf == NULL );
	sSelf = this;
}

MacGitver::~MacGitver()
{
	setRepository( Git::Repository() );

	delete mModules;
	sSelf = NULL;
}

Modules* MacGitver::modules()
{
	return mModules;
}

FSWatcher* MacGitver::watcher()
{
	return mWatcher;
}

Git::Repository MacGitver::repository() const
{
	return mRepository;
}

void MacGitver::setRepository( const Git::Repository& repo )
{
	mRepository = repo;

	mWatcher->setRepository( repo );
	mModules->repositoryChanged( repo );

	emit repositoryChanged( mRepository );
}

IMainWindow* MacGitver::mainWindow()
{
	Q_ASSERT( mMainWindow );
	return mMainWindow;
}

void MacGitver::setMainWindow( IMainWindow* mainWindow )
{
	Q_ASSERT( !mMainWindow );
	mMainWindow = mainWindow;

	mModules->initialize();
}

MacGitver* MacGitver::sSelf = NULL;

MacGitver& MacGitver::self()
{
	Q_ASSERT( sSelf );
	return *sSelf;
}
