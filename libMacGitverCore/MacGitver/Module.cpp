/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/MacGitver/Module.h"

Module::Module()
{
    ConfigDialog::registerProvider(this);
}

Module::~Module()
{
    ConfigDialog::unregisterProvider(this);
}

int Module::configPagePriority() const
{
    return 500;
}

void Module::registerView( const Heaven::ViewIdentifier& identifier, const QString& displayName,
                           MgvViewCreator creator )
{
    MacGitver::self().registerView( identifier, displayName, creator );
}

void Module::unregisterView( const Heaven::ViewIdentifier& identifier )
{
    MacGitver::self().unregisterView( identifier );
}

void Module::setupConfigPages( ConfigDialog* dialog )
{
    Q_UNUSED( dialog );
}
