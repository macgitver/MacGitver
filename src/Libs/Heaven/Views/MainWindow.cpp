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

#include <QResizeEvent>

#include "Mode.h"
#include "MainWindow.h"
#include "MainWindowPrivate.h"

namespace Heaven
{

	MainWindowPrivate::MainWindowPrivate()
	{
		mOwner = NULL;
		mCurrentMode = NULL;
	}

	void MainWindowPrivate::setup()
	{
	}

	void MainWindowPrivate::switchToMode( Mode* mode )
	{
		if( mode == mCurrentMode )
		{
			return;
		}

		if( mCurrentMode )
		{
			mCurrentMode->deactivate();
			mCurrentMode = NULL;
		}

		if( mode )
		{
			mode->activate();
			mCurrentMode = mode;
		}

		emit mOwner->currentModeChanged( mode );
	}


	MainWindow::MainWindow()
		: QMainWindow()
		, d( new MainWindowPrivate )
	{
		d->mOwner = this;
		d->setup();
	}

	MainWindow::~MainWindow()
	{
		delete d;
	}

	void MainWindow::setCurrentMode( Mode* mode )
	{
		d->switchToMode( mode );
	}

	void MainWindow::addMode( Mode* mode )
	{
		d->mModes.append( mode );
	}

	void MainWindow::removeMode( Mode* mode )
	{
		if( currentMode() == mode )
		{
			// FIXME: Find a suitable mode
			d->switchToMode( NULL );
		}

		int i = d->mModes.indexOf( mode );
		Q_ASSERT( i != -1 );
		d->mModes.remove( i );
		delete mode;
	}

	Mode* MainWindow::currentMode()
	{
		return d->mCurrentMode;
	}

}
