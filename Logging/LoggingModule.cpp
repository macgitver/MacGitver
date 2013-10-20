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

#include "LoggingModule.hpp"
#include "LoggingView.hpp"

LoggingModule::LoggingModule()
{
}

Heaven::View* LoggingModule::createLoggingView()
{
    return new LoggingView();
}

void LoggingModule::initialize()
{
    registerView( "Log",
                  tr( "Log" ),
                  &LoggingModule::createLoggingView );
}

void LoggingModule::deinitialize()
{
    unregisterView( "Log" );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Logging, LoggingModule )
#endif
