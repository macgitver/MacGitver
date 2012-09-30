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

#include "Model.hpp"

ConfigSetting::ConfigSetting( QDomElement el )
{
	mName = el.attribute( QLatin1String( "Name" ) );
	mType = el.attribute( QLatin1String( "Type" ) );
	mSubType = el.attribute( QLatin1String( "Subtype" ), QString() );
	mValidatorRule = el.attribute( QLatin1String( "Validate" ), QString() );
}

QString ConfigSetting::name() const
{
	return mName;
}

VariantType ConfigSetting::type() const
{
	return VariantTypes::self().typeFor( mType, mSubType );
}

QString ConfigSetting::typeName() const
{
	return mType;
}

QString ConfigSetting::subTypeName() const
{
	return mSubType;
}

QString ConfigSetting::validatorRule() const
{
	return mValidatorRule;
}
