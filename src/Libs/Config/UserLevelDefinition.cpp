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

#include "UserLevelDefinition.h"

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
