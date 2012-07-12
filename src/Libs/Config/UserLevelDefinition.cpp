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

#include <QSet>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QDomDocument>

#include "UserLevelDefinition.h"


EnableDisable::EnableDisable()
{
}

EnableDisable::EnableDisable( const QString& regex, bool enable )
	: mRegex( regex )
	, mEnable( enable )
{
}

QString EnableDisable::regex() const
{
	return mRegex;
}

bool EnableDisable::enable() const
{
	return mEnable;
}

EnableDisableList::EnableDisableList()
{
}

bool EnableDisableList::read( const QDomElement& parent )
{
	QDomElement e = parent.firstChildElement();
	while( e.isElement() )
	{
		if( e.tagName() == "Enable" )
		{
			mList.append( EnableDisable( e.attribute( "Name" ), true ) );
		}
		else if( e.tagName() == "Disable" )
		{
			mList.append( EnableDisable( e.attribute( "Name" ), false ) );
		}
		else
		{
			return false;
		}

		e = e.nextSiblingElement();
	}

	return true;
}

int EnableDisableList::count() const
{
	return mList.count();
}

const EnableDisable& EnableDisableList::at( int index ) const
{
	return mList.at( index );
}

QStringList EnableDisableList::appliedTo( const QStringList& list ) const
{
	QSet< QString > result;

	for( int i = 0; i < mList.count(); i++ )
	{
		QRegExp re( mList.at( i ).regex() );
		for( int j = 0; j < list.count(); j++ )
		{
			QString search = list.at( j );
			if( re.indexIn( search ) != -1 )
			{
				if( mList.at( i ).enable() )
				{
					if( !result.contains( search ) )
					{
						result.insert( search );
					}
				}
				else
				{
					if( result.contains( search ) )
					{
						result.remove( search );
					}
				}
			}
		}
	}

	return result.toList();
}

UserLevelMode::UserLevelMode()
{
}

UserLevelMode::UserLevelMode( const QString& modeName )
	: mModeName( modeName )
{
}

QString UserLevelMode::name() const
{
	return mModeName;
}

EnableDisableList& UserLevelMode::allowedViews()
{
	return mAllowedViews;
}

const EnableDisableList& UserLevelMode::allowedViews() const
{
	return mAllowedViews;
}


UserLevelDefinition::UserLevelDefinition( const QString& name, int appLevel, int precedence )
{
	mName = name;
	mAppLevel = appLevel;
	mPrecedence = precedence;
}

void UserLevelDefinition::setDescription( const QString& desc )
{
	mDescription = desc;
}

void UserLevelDefinition::addPreset( const QString& type, const QString& preset )
{
	mHeavenPresets.insert( type, preset );
}

QString UserLevelDefinition::name() const
{
	return mName;
}

int UserLevelDefinition::precedence() const
{
	return mPrecedence;
}

QString UserLevelDefinition::description() const
{
	return mDescription;
}

QString UserLevelDefinition::preset( const QString& type ) const
{
	if( mHeavenPresets.contains( type ) )
	{
		return mHeavenPresets.value( type );
	}

	if( mHeavenPresets.contains( "*" ) )
	{
		return mHeavenPresets[ "*" ];
	}

	return QString();
}

void UserLevelDefinition::readGuiDef( const QString& fileName )
{
	QFile f( fileName );
	if( !f.open( QFile::ReadOnly ) )
	{
		return;
	}

	QDomDocument doc;
	doc.setContent( &f );

	QDomElement elMode = doc.documentElement().firstChildElement( "Mode" );

	while( elMode.isElement() )
	{
		UserLevelMode mode( elMode.attribute( "Name" ) );

		QDomElement elViews = elMode.firstChildElement( "Views" );
		if( elViews.isElement() )
		{
			mode.allowedViews().read( elViews );
		}

		mModes.append( mode );

		elMode = elMode.nextSiblingElement( "Mode" );
	}
}

int UserLevelDefinition::numModes() const
{
	return mModes.count();
}

UserLevelMode UserLevelDefinition::mode( int index ) const
{
	return mModes.at( index );
}

UserLevelMode UserLevelDefinition::mode( const QString& name ) const
{
	for( int i = 0; i < mModes.count(); i++ )
	{
		if( mModes[ i ].name() == name )
		{
			return mModes[ i ];
		}
	}

	return UserLevelMode();
}
