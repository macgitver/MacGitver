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

#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libMacGitverCore/App/MacGitver.hpp"

#if QT_VERSION < 0x050000
#define Q_PLUGIN_METADATA(x)
#endif

class ConfigDialog;

/**
 * @brief The Module class provides an abstract implementation for application plugins.
 * Use this, when implementing new plugins.
 */
class MGV_CORE_API Module : public QObject
{
    Q_OBJECT
public:
    Module();

public:
    /**
     * @brief Informs a module about the currently referenced repository has changed.
     * @param newRepository
     */
    virtual void repositoryChanged( Git::Repository newRepository );

    /**
     * @brief Setup a configuration dialog for a module, which is used in the settings.
     * @param dialog
     */
    virtual void setupConfigPages( ConfigDialog* dialog );

    /**
     * @brief Tells the module to initialize itself, after it was loaded and instantiated.
     */
    virtual void initialize() = 0;

    /**
     * @brief Called before a module is unloaded, telling it to clean up itself.
     */
    virtual void deinitialize() = 0;

protected:
    /**
     * @brief       Register a View to the application.
     *
     * @param[in]   identifier  The key to access this module's view (must be unique)
     *
     * @param[in]   displayName A translated name that is displayed to the user when this view is
     *              referenced.
     *
     * @param[in]   creator     A call back function that actually creates a View of this type.
     */
    void registerView( const Heaven::ViewIdentifier& identifier, const QString& displayName,
                       MgvViewCreator creator );

    /**
     * @brief Unregister a previously registered module view.
     * @param identifier the view identifier used to register the view
     */
    void unregisterView( const Heaven::ViewIdentifier& identifier );
};

Q_DECLARE_INTERFACE( Module,
                     "org.macgitver.Module/0.1" )

#endif
