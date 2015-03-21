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

#include "libMacGitverCore/App/MacGitver.hpp"

#include "SubmodulesModule.h"
#include "SubmodulesView.h"

SubmodulesModule::SubmodulesModule()
{
}

Heaven::View* SubmodulesModule::createSubmodulesView()
{
    return new SubmodulesView;
}

void SubmodulesModule::initialize()
{
    registerView( "Submodules", tr( "Submodules" ),
                  & SubmodulesModule::createSubmodulesView );
}

void SubmodulesModule::deinitialize()
{
    unregisterView( "Submodules" );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Submodules, SubmodulesModule )
#endif
