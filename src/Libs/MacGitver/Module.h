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

#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "libGitWrap/Repository.h"

#include "Interfaces/IModule.h"
#include "Interfaces/IMainWindow.h"

#include "MacGitver/MacGitverApi.h"
#include "MacGitver/MacGitver.h"

#if QT_VERSION < 0x050000
#define Q_PLUGIN_METADATA(x)
#endif

/**
 * @brief The Module class provides an abstract implementation for application plugins.
 * Use this, when implementing new plugins.
 */
class MGV_CORE_API Module : public QObject, public IModule
{
	Q_INTERFACES( IModule )
	Q_OBJECT
public:
	Module();

public:
	IMainWindow* mainWindow();

public:
	void repositoryChanged( Git::Repository newRepository );

protected:
    /**
     * @brief If the module has a view, it can register it here to the application.
     * @param identifier the key to access this module's view (must be unique)
     * @param type the view type, to specify the behaviour
     * @param creator the creator used to setup the view
     */
    void registerView( const QString& identifier, Heaven::ViewTypes type, ViewCreator* creator );

    /**
     * @brief Unregister a previously registered module view.
     * @param identifier the view identifier used to register the view
     */
	void unregisterView( const QString& identifier );
};

#endif
