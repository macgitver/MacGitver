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
#include <QFileDialog>

#include "MacGitver/MacGitver.h"

#include "RemoteCreateEditDlg.h"
#include "RemotesModule.h"
#include "RemotesView.h"

RemotesModule::RemotesModule()
{
}

void RemotesModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types RemotesModule::providesModuleTypes() const
{
	return View;
}

Heaven::View* RemotesModule::createRemotesView()
{
	return new RemotesView;
}

void RemotesModule::initialize()
{
	setupActions( this );
	acRemotesAC->mergeInto( "RemotesMP" );

	MacGitver::self().registerView( QLatin1String( "Remotes" ), Heaven::GlobalViewType,
									&RemotesModule::createRemotesView );
}

void RemotesModule::deinitialize()
{
	MacGitver::self().unregisterView( QLatin1String( "Remotes" ) );
}

void RemotesModule::onRemoteCreate()
{
	RemoteCreateEditDlg().exec();
}

Q_EXPORT_PLUGIN2( Remotes, RemotesModule )
