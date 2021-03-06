/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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

#include <QApplication>
#include <QPluginLoader>
#include <QStringBuilder>
#include <QDir>

#include "libBlueSky/Application.hpp"

#include "MacGitver/Modules.h"

Modules::Modules()
{
}

Modules::~Modules()
{
    foreach(Module* module, mModules) {
        if (module) {
            module->deinitialize();
            delete module;
        }
    }

    mModules.clear();
}

void Modules::search( const QDir& binDir )
{
    QStringList modFiles;
    modFiles << QStringLiteral( "Mod*.mgv" );

    // qDebug( "Searching for Modules in %s", qPrintable( binDir.absolutePath() ) );

    foreach( QString modName, binDir.entryList( modFiles ) )
    {
        QPluginLoader* loader = new QPluginLoader( binDir.filePath( modName ), this );

        QObject* o = loader->instance();
        if( !o )
        {
            qDebug( "Failed to load a module\n%s: %s",
                    qPrintable( modName ),
                    qPrintable( loader->errorString() ) );
        }

        Module* mod = qobject_cast< Module* >( o );
        if( mod )
        {
            addModule( mod );
        }
        else
        {
            qDebug( "The plugin '%s' does not implment the Module interface",
                    qPrintable( modName ) );

            delete loader;
        }
    }
}

void Modules::initialize()
{
    QDir binDir( qApp->applicationDirPath() );
    search( binDir );

    binDir = QDir( qApp->applicationDirPath() % QStringLiteral( "/modules" ) );
    search( binDir );
    binDir.cdUp();

    #ifdef Q_OS_UNIX
    binDir = QDir( qApp->applicationDirPath() % QStringLiteral( "/../libexec/MacGitver" ) );
    search( binDir );
    #endif

    setupInternals();

    foreach (Module* module, mModules) {
        module->initialize();
    }

    BlueSky::Application::instance()->setupModes();
}

void Modules::setupInternals()
{
    QObjectList ol = QPluginLoader::staticInstances();

    foreach( QObject* o, ol )
    {
        Module* mod = qobject_cast< Module* >( o );
        if( mod )
        {
            addModule( mod );
        }
    }
}

void Modules::addModule( Module *mod )
{
    mModules.insert( mod );
}

void Modules::delModule( Module *mod )
{
    mModules.remove( mod );
}
