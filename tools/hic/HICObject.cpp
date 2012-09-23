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

#include "HICObject.h"

HICObject::HICObject( ObjectTypes type )
	: mType( type )
{
}

ObjectTypes HICObject::type()
{
	return mType;
}

void HICObject::setName( const QString& name )
{
	mName = name;
}

QString HICObject::name() const
{
	return mName;
}

bool HICObject::hasProperty( const QString& name, HICPropertyTypes type ) const
{
	if( type == HICP_Any )
	{
		return mProperties.contains( name );
	}

	if( mProperties.contains( name ) )
	{
		const HICProperty& p = mProperties[ name ];
		return p.type() == type;
	}

	return false;
}

HICProperty HICObject::getProperty( const QString& name ) const
{
	return mProperties.value( name, HICProperty() );
}

QStringList HICObject::propertyNames() const
{
	return mProperties.keys();
}

void HICObject::addProperty( QString name, HICProperty p )
{
	Q_ASSERT( !mProperties.contains( name ) );
	mProperties.insert( name, p );
}

void HICObject::addContent( HICObject* content )
{
	mContent.append( content );
}

HICObjects HICObject::content() const
{
	return mContent;
}

HICObjects HICObject::content( ObjectTypes type ) const
{
	return mContent.byType( type );
}

bool HICObject::hasReferenceTo( HICObject* obj ) const
{
	return mContent.contains( obj );
}

HICObjects::HICObjects()
{
}

HICObjects HICObjects::byType( ObjectTypes type ) const
{
	HICObjects result;

	foreach( HICObject* obj, *this )
	{
		if( obj->type() == type )
		{
			result.append( obj );
		}
	}

	return result;
}

HIDModel::HIDModel()
{
}

HIDObjectHash& HIDModel::objects()
{
	return mObjects;
}

const HIDObjectHash& HIDModel::objects() const
{
	return mObjects;
}

HICObjects HIDModel::allObjects( ObjectTypes byType ) const
{
	HICObjects result;

	foreach( HICObject* obj, mObjects )
	{
		if( obj->type() == byType )
		{
			result.append( obj );
		}
	}

	return result;
}
