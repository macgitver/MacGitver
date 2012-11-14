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

#ifndef MGV_I_MODULE_H
#define MGV_I_MODULE_H

#include <QtPlugin>
#include <QFlags>

#include "Interfaces/InterfaceApi.h"

class IConfigDialog;

namespace Git
{
    class Repository;
}

/**
 * @brief The IModule class provides an interface for implementing application plugins.
 * This interface is not used directly. Use the abstract class @see Module instead.
 */
class INTERFACES_API IModule
{
public:
    IModule();
    virtual ~IModule();

public:
    /**
     * @brief Informs a module about the currently referenced repository has changed.
     * @param newRepository
     */
    virtual void repositoryChanged( Git::Repository newRepository ) = 0;

    /**
     * @brief Setup a configuration dialog for a module, which is used in the settings.
     * @param dialog
     */
    virtual void setupConfigPages( IConfigDialog* dialog ) = 0;

    /**
     * @brief Tells the module to initialize itself, after it was loaded and instantiated.
     */
    virtual void initialize() = 0;

    /**
     * @brief Called before a module is unloaded, telling it to clean up itself.
     */
    virtual void deinitialize() = 0;
};

Q_DECLARE_INTERFACE( IModule,
                     "org.babbelbox.sacu.macgitver.IModule/0.1" )

#endif
