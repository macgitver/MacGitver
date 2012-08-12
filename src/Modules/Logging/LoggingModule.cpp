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

#include "LoggingModule.h"
#include "LoggingView.h"

LoggingModule::LoggingModule()
{
}

void LoggingModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types LoggingModule::providesModuleTypes() const
{
	return View;
}

Heaven::View* LoggingModule::createLoggingView()
{
	return new LoggingView();
}

void LoggingModule::initialize()
{
	MacGitver::self().setLog( this );

	registerView( QLatin1String( "Log" ),
				  Heaven::GlobalViewType,
				  &LoggingModule::createLoggingView );
}

void LoggingModule::deinitialize()
{
	MacGitver::self().setLog( NULL );
	unregisterView( QLatin1String( "Log" ) );
}

void LoggingModule::addMessage( LogType type, const QString& message )
{
	qDebug( "%i: %s", int(type), qPrintable( message ) );
}

Q_EXPORT_PLUGIN2( Logging, LoggingModule )
