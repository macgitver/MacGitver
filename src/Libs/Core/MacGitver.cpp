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

#include <QPlastiqueStyle>

#include "GitWrap/Git.h"

#include "Libs/Core/MacGitver.h"
#include "Libs/Core/Modules.h"
#include "Libs/Core/MainWindow.h"

MacGitver::MacGitver( int argc, char** argv )
	: QApplication( argc, argv )
	, mMainWindow( NULL )
{
	Git::initLibGit();

	setOrganizationName( "SaCu" );
	setApplicationName( "MacGitver" );
	setStyle( new QPlastiqueStyle );

	mModules = new Modules( this );

	Q_ASSERT( sSelf == NULL );
	sSelf = this;

	mMainWindow = new MainWindow;
	mMainWindow->show();

	mModules->initialize();
}

MacGitver::~MacGitver()
{
	closeRepository();

	delete mModules;

	sSelf = NULL;
}

Modules* MacGitver::modules()
{
	return mModules;
}

Git::Repository MacGitver::repository() const
{
	return mRepository;
}

void MacGitver::closeRepository()
{
	if( mRepository.isValid() )
	{
		setRepository( Git::Repository() );
	}
}

void MacGitver::openedRepository( const Git::Repository& repo )
{
	setRepository( repo );
}

void MacGitver::setRepository( const Git::Repository& repo )
{
	mRepository = repo;

	mModules->repositoryChanged( repo );

	emit repositoryChanged( mRepository );
}

MainWindow* MacGitver::mainWindow()
{
	Q_ASSERT( mMainWindow );
	return mMainWindow;
}

MacGitver* MacGitver::sSelf = NULL;

MacGitver& MacGitver::self()
{
	Q_ASSERT( sSelf );
	return *sSelf;
}
