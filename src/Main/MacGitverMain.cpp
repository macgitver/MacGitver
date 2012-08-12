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

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>
#include <QTextStream>
#include <QDomDocument>

#include "MacGitver/Modules.h"
#include "Config/Config.h"
#include "Config/UserLevelDefinition.h"

#include "MacGitverMain.h"
#include "MainWindow.h"

MacGitverMain::MacGitverMain( int argc, char** argv )
	: MacGitver( argc, argv )
{
}

void MacGitverMain::loadModules()
{
	QDir binDir( applicationDirPath() );

	QStringList modFiles;
	modFiles << QLatin1String( "Mod*.mgv" );
	foreach( QString modName, binDir.entryList( modFiles ) )
	{
		QPluginLoader* loader = new QPluginLoader( binDir.filePath( modName ), this );
		QObject* o = loader->instance();
		if( !o )
			qDebug( "%s: %s", qPrintable( modName ), qPrintable( loader->errorString() ) );

		Module* mod = qobject_cast< Module* >( o );
		if( mod )
		{
			modules()->addModule( mod );
		}
		else
		{
			delete loader;
		}
	}

	modules()->initialize();
}

void MacGitverMain::loadLevels()
{
	Config::self().loadLevels( QLatin1String( ":/Xml/Levels.xml" ) );
}

void MacGitverMain::boot()
{
	loadLevels();
	loadModules();

	MainWindow* mw = new MainWindow;
	setMainWindow( mw );

	mw->show();
}

int MacGitverMain::exec()
{
	QMetaObject::invokeMethod( this, "boot", Qt::QueuedConnection );
	return MacGitver::exec();
}
