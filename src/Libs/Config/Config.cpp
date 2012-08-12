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

#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QSettings>

#include "Config/Config.h"

Config::Config()
	: mSettings( NULL )
{
}

Config::~Config()
{
	// QSettings' DTor will sync()
	delete mSettings;
}

Config* Config::sSelf = NULL;

Config& Config::self()
{
	if( !sSelf )
	{
		sSelf = new Config;
	}
	return *sSelf;
}

void Config::loadLevels( const QString& fileName )
{
	QFile f( fileName );
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
		Q_ASSERT( e2.tagName() == QLatin1String( "level" ) );
		UserLevelDefinition::Ptr lvldef = UserLevelDefinition::read( e2 );
		if( !lvldef )
		{
			return /*false*/;
		}
		addUserLevel( lvldef );

		e2 = e2.nextSiblingElement();
	}
}

void Config::addUserLevel( UserLevelDefinition::Ptr level )
{
	int i = 0;
	while( i < mLevels.count() )
	{
		if( mLevels[ i ]->precedence() < level->precedence() )
			i++;
		else
			break;
	}

	mLevels.insert( i, level );
}

QList< UserLevelDefinition::Ptr > Config::levels() const
{
	return mLevels;
}

void Config::loadSettings()
{
	delete mSettings;
	mSettings = new QSettings();
}

void Config::saveSettings()
{
	if( mSettings )
	{
		mSettings->sync();
	}
}

QVariant Config::get( const char* szPath, const char* szDefaultValue ) const
{
	return get( szPath, QLatin1String( szDefaultValue ) );
}

QVariant Config::get( const char* szPath, const QVariant& defaultValue ) const
{
	return get( QLatin1String( szPath ), defaultValue );
}

QVariant Config::get( const QString& path, const QVariant& defaultValue ) const
{
	if( mSettings )
	{
		return mSettings->value( path, defaultValue );
	}

	const_cast< Config* >( this )->loadSettings();

	if( mSettings )
	{
		return mSettings->value( path, defaultValue );
	}

	return defaultValue;
}

void Config::set( const QString& path, const QVariant& value )
{
	if( !mSettings )
	{
		loadSettings();
	}
	Q_ASSERT( mSettings );

	mSettings->setValue( path, value );
}

