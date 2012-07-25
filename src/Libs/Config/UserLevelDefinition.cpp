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

EnableDisableList::EnableDisableList( const QDomElement& parent )
{
	if( !read( parent ) )
	{
		mList.clear();
	}
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

UserLevelDefaultLayoutEntry::UserLevelDefaultLayoutEntry()
{
}

UserLevelDefaultLayoutEntry::~UserLevelDefaultLayoutEntry()
{
}

UserLevelDefaultLayoutEntry::Type UserLevelDefaultLayoutEntry::type() const
{
	return mType;
}

int UserLevelDefaultLayoutEntry::numChildren() const
{
	return mChildren.count();
}

UserLevelDefaultLayoutEntry::Ptr UserLevelDefaultLayoutEntry::childAt( int index ) const
{
	return mChildren.at( index );
}

QVector< UserLevelDefaultLayoutEntry::Ptr > UserLevelDefaultLayoutEntry::children() const
{
	return mChildren;
}

bool UserLevelDefaultLayoutEntry::isVertical() const
{
	return mVertical;
}

UserLevelDefaultLayoutEntry::TabPos UserLevelDefaultLayoutEntry::tabPos() const
{
	return mTabPos;
}

QString UserLevelDefaultLayoutEntry::name() const
{
	return mName;
}

bool UserLevelDefaultLayoutEntry::parseOrient( const QString& s )
{
	return s == "Vert";
}

UserLevelDefaultLayoutEntry::TabPos UserLevelDefaultLayoutEntry::parseCaption( const QString& s )
{
	if( s == "Left" )
		return Left;
	if( s == "Right" )
		return Right;
	if( s == "Bottom" )
		return Bottom;

	return Top;
}

UserLevelDefaultLayoutEntry::Ptr UserLevelDefaultLayoutEntry::read( const QDomElement& el )
{
	UserLevelDefaultLayoutEntry::Ptr entry( new UserLevelDefaultLayoutEntry );

	if( el.tagName() == "Split" )
	{
		entry->mType = Split;
		entry->mVertical = parseOrient( el.attribute( "Orient" ) );
		entry->mStretch = el.attribute( "Stretch", "0" ).toInt();
	}
	else if( el.tagName() == "Tab" )
	{
		entry->mType = Tab;
		entry->mTabPos = parseCaption( el.attribute( "Caption" ) );
		entry->mStretch = el.attribute( "Stretch", "0" ).toInt();
	}
	else if( el.tagName() == "View" )
	{
		entry->mType = View;
		entry->mName = el.attribute( "Name" );
		entry->mStretch = el.attribute( "Stretch", "0" ).toInt();
	}
	else
	{
		return UserLevelDefaultLayoutEntry::Ptr();
	}

	for( QDomElement e = el.firstChildElement(); e.isElement(); e = e.nextSiblingElement() )
	{
		UserLevelDefaultLayoutEntry::Ptr child = read( e );
		if( !child )
		{
			return UserLevelDefaultLayoutEntry::Ptr();
		}
		entry->mChildren.append( child );
	}

	return entry;
}

UserLevelDefaultLayout::UserLevelDefaultLayout()
{
}

UserLevelDefaultLayout::~UserLevelDefaultLayout()
{
}

UserLevelDefaultLayoutEntry::Ptr UserLevelDefaultLayout::root() const
{
	return mRoot;
}

UserLevelDefaultLayout::Ptr UserLevelDefaultLayout::read( const QDomElement& el )
{
	UserLevelDefaultLayout::Ptr deflay( new UserLevelDefaultLayout );

	QDomElement elDef = el.firstChildElement();
	if( !elDef.isElement() )
	{
		return UserLevelDefaultLayout::Ptr();
	}

	deflay->mRoot = UserLevelDefaultLayoutEntry::read( elDef );

	return deflay;
}

UserLevelMode::UserLevelMode()
{
}

UserLevelMode::UserLevelMode( const QString& modeName )
	: mModeName( modeName )
	, mIsLocking( false )
	, mIsUserSelectable( true )
{
}

UserLevelMode::~UserLevelMode()
{
}

QString UserLevelMode::name() const
{
	return mModeName;
}

UserLevelDefaultLayout::Ptr UserLevelMode::defaultLayout() const
{
	return mDefaultLayout;
}

EnableDisableList& UserLevelMode::allowedViews()
{
	return mAllowedViews;
}

const EnableDisableList& UserLevelMode::allowedViews() const
{
	return mAllowedViews;
}

bool UserLevelMode::isLockingMode() const
{
	return mIsLocking;
}

bool UserLevelMode::isUserSelectable() const
{
	return mIsUserSelectable;
}


UserLevelMode::Ptr UserLevelMode::read( const QDomElement& el )
{
	UserLevelMode::Ptr mode( new UserLevelMode( el.attribute( "Name" ) ) );

	if( el.hasAttribute( "Selectable" ) )
	{
		if( el.attribute( "Selectable" ) == "0" )
		{
			mode->mIsUserSelectable = false;
		}
	}

	if( el.hasAttribute( "Locking" ) )
	{
		if( el.attribute( "Locking" ) == "1" )
		{
			mode->mIsLocking = true;
		}
	}

	for( QDomElement e = el.firstChildElement(); e.isElement(); e = e.nextSiblingElement() )
	{
		if( e.tagName() == "Views" )
		{
			mode->mAllowedViews = EnableDisableList( e );
		}
		else if( e.tagName() == "DefaultLayout" )
		{
			mode->mDefaultLayout = UserLevelDefaultLayout::read( e );
		}
	}

	return mode;
}

UserLevelDefinition::UserLevelDefinition()
{
}

UserLevelDefinition::~UserLevelDefinition()
{
}

UserLevelDefinition::Ptr UserLevelDefinition::read( const QDomElement& el )
{
	UserLevelDefinition::Ptr def( new UserLevelDefinition );

	def->mName = el.attribute( "name" );
	def->mId = el.attribute( "id" );
	def->mAppLevel = el.attribute( "applevel" ).toInt();
	def->mPrecedence = el.attribute( "precedence" ).toInt();

	QString foo;
	QTextStream ts( &foo );
	el.firstChildElement( "desc" ).save( ts, 0 );

	def->mDescription = foo.replace( "<desc", "<html" ).simplified();

	QDomElement e = el.firstChildElement( "gui" );
	if( e.isElement() )
	{
		def->readGuiDef( e.attribute( "file" ) );
	}

	return def;
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

QString UserLevelDefinition::id() const
{
	return mId;
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

	QDomElement el = doc.documentElement().firstChildElement();

	while( el.isElement() )
	{
		if( el.tagName() == "Mode" )
		{
			UserLevelMode::Ptr mode = UserLevelMode::read( el );
			if( !mode )
			{
				return;
			}
			mModes.append( mode );
		}
		else if( el.tagName() == "Preset" )
		{
			QString preset = el.attribute( "Name" );
			QString mode = el.attribute( "Mode" );
			mHeavenPresets.insert( preset, mode );
		}

		el = el.nextSiblingElement();
	}
}

int UserLevelDefinition::numModes() const
{
	return mModes.count();
}

UserLevelMode::Ptr UserLevelDefinition::mode( int index ) const
{
	return mModes.at( index );
}

UserLevelMode::Ptr UserLevelDefinition::mode( const QString& name ) const
{
	for( int i = 0; i < mModes.count(); i++ )
	{
		if( mModes[ i ]->name() == name )
		{
			return mModes[ i ];
		}
	}

	return UserLevelMode::Ptr();
}

QList< UserLevelMode::Ptr > UserLevelDefinition::allModes() const
{
	return mModes;
}

QList< UserLevelMode::Ptr > UserLevelDefinition::userModes() const
{
	QList< UserLevelMode::Ptr > result;

	for( int i = 0; i < mModes.count(); i++ )
	{
		UserLevelMode::Ptr m = mModes.at( i );

		if( m->isUserSelectable() )
		{
			result += m;
		}
	}

	return result;
}
