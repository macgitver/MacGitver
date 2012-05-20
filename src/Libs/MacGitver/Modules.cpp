/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include <QPluginLoader>

#include "MacGitver/Modules.h"

Modules::Modules( QObject* parent )
	: QObject( parent )
{
}

Modules::~Modules()
{
	foreach( Module* module, mModules )
	{
		module->repositoryChanged( Git::Repository() );
		delete module;
	}
}

void Modules::initialize()
{
	setupInternals();

	foreach( Module* module, mModules )
	{
		module->initialize();
	}
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

#if 0
	int i = 0;
	while( sInternals[ i ].szName )
	{
		qDebug( "Setting up module %s.", sInternals[ i ].szName );

		Module* m = sInternals[ i ].creator();
		addModule( m );

		i++;
	}
#endif
}

void Modules::repositoryChanged( Git::Repository newRepository )
{
	foreach( Module* module, mModules )
	{
		module->repositoryChanged( newRepository );
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

void Modules::setupConfigPages( IConfigDialog* dlg )
{
	foreach( Module* m, mModules )
	{
		m->setupConfigPages( dlg );
	}
}
