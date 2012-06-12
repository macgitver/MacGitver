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
#include "MacGitver/UserLevelDefinition.h"

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
	modFiles << "Mod*.mgv";
	foreach( QString modName, binDir.entryList( modFiles ) )
	{
		QPluginLoader* loader = new QPluginLoader( binDir.filePath( modName ), this );
		QObject* o = loader->instance();
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
}

void MacGitverMain::loadLevels()
{
	QFile f( ":/Xml/levels.xml" );
	if( !f.open( QFile::ReadOnly ) )
	{
		qFatal( "Cannot load levels.xml" );
		return;
	}

	QDomDocument doc;
	doc.setContent( &f );

	QDomElement e1 = doc.documentElement();
	QDomElement e2 = e1.firstChildElement();
	while( e2.isElement() )
	{
		Q_ASSERT( e2.tagName() == "level" );
		UserLevelDefinition* li = new UserLevelDefinition(
									  e2.attribute( "name" ),
									  e2.attribute( "applevel" ).toInt(),
									  e2.attribute( "precedence" ).toInt() );

		QString foo;
		QTextStream ts( &foo );
		e2.firstChildElement( "desc" ).save( ts, 0 );

		li->setDescription( foo.replace( "<desc", "<html" ).simplified() );

		addUserLevel( li );

		e2 = e2.nextSiblingElement();
	}
}

void MacGitverMain::boot()
{
	qDebug() << arguments();
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
