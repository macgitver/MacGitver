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

#include "WelcomeModule.h"
#include "WelcomeView.h"

WelcomeModule::WelcomeModule()
{
}

Heaven::View* WelcomeModule::createWelcomeView()
{
    return new WelcomeView();
}

void WelcomeModule::initialize()
{
    registerView( "Welcome",
                  tr( "Welcome" ),
                  &WelcomeModule::createWelcomeView );
}

void WelcomeModule::deinitialize()
{
    unregisterView( "Welcome" );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Welcome, WelcomeModule )
#endif
