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

#include "Config/Config.h"
#include "Config/UserLevelDefinition.h"

Config::Config()
{
}

Config::~Config()
{
	qDeleteAll( mLevels );
	mLevels.clear();
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

void Config::addUserLevel( UserLevelDefinition* level )
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

QList< UserLevelDefinition* > Config::levels() const
{
	return mLevels;
}
