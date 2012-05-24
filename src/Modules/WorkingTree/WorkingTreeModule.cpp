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

#include <QtPlugin>

#include "Interfaces/IMainWindow.h"

#include "WorkingTreeModule.h"
#include "IndexWidget.h"

WorkingTreeModule::WorkingTreeModule()
	: mView( NULL )
{
}

void WorkingTreeModule::repositoryChanged( Git::Repository newRepository )
{
	if( mView )
	{
		mView->repositoryChanged( newRepository );
	}
}

void WorkingTreeModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types WorkingTreeModule::providesModuleTypes() const
{
	return View;
}

void WorkingTreeModule::initialize()
{
	mView = new IndexWidget();
	mainWindow()->integrateView( mView, Heaven::Central );
}

void WorkingTreeModule::deinitialize()
{
	delete mView;
	mView = NULL;
}

Q_EXPORT_PLUGIN2( WorkingTree, WorkingTreeModule )
