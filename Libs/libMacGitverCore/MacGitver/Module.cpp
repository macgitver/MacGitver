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

#include "libBlueSky/Application.hpp"

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

/**
 * @brief       Register a View to the application.
 *
 * @param[in]   identifier  The key to access this module's view (must be unique)
 *
 * @param[in]   displayName A translated name that is displayed to the user when this view is
 *                          referenced.
 *
 * @param[in]   creator     A call back function that actually creates a View of this type.
 *
 * @return      The descriptor for the new view.
 *
 */
BlueSky::ViewDescriptor* Module::registerView(const BlueSky::ViewIdentifier& identifier,
                                              const QString& displayName,
                                              MgvViewCreator creator)
{
    return MacGitver::self().registerView(identifier, displayName, creator);
}

/**
 * @brief       Unregister a previously registered module view.
 *
 * @param[in]   identifier      The view identifier that was used to register the view.
 *
 */
void Module::unregisterView(const BlueSky::ViewIdentifier& identifier) {
    MacGitver::self().unregisterView(identifier);
}

/**
 * @brief       Register a mode in libBlueSky
 *
 * @param[in]   mode        The mode to register.
 *
 */
void Module::registerMode(BlueSky::Mode* mode) {
    BlueSky::Application::instance()->addMode(mode);
}

/**
 * @brief       Unregister a mode from libBlueSky
 *
 * @param[in]   mode        The mode to unregister.
 *
 */
void Module::unregisterMode(BlueSky::Mode* mode) {
    BlueSky::Application::instance()->removeMode(mode);
}

void Module::setupConfigPages( ConfigDialog* dialog )
{
    Q_UNUSED( dialog );
}
