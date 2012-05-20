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

#define QT_STATICPLUGIN
#include <QtPlugin>

#include "Interfaces/IMainWindow.h"

#include "Modules/History/HistoryModule.h"
#include "Modules/History/HistoryView.h"

HistoryModule::HistoryModule()
	: mView( NULL )
{
}

void HistoryModule::repositoryChanged( Git::Repository newRepository )
{
	if( mView )
	{
		mView->repositoryChanged( newRepository );
	}
}

void HistoryModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types HistoryModule::providesModuleTypes() const
{
	return View;
}

void HistoryModule::initialize()
{
	mView = new HistoryView();
	mainWindow()->integrateView( mView, Heaven::Central );
}

void HistoryModule::deinitialize()
{
	delete mView;
	mView = NULL;
}

Q_EXPORT_STATIC_PLUGIN2( History, HistoryModule )
Q_IMPORT_PLUGIN( History )
