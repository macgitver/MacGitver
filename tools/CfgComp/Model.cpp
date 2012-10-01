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
#include <QStringBuilder>

#include "Model.hpp"

QString utf8Encoded( QString str )
{
	return str;
}

ConfigSetting::ConfigSetting( QDomElement el, ConfigSubSection* parent )
{
	mSubSection = parent;

	mName = el.attribute( QLatin1String( "Name" ) );
	mDefaultValue = el.attribute( QLatin1String( "Default" ), QString() );
	mType = el.attribute( QLatin1String( "Type" ) );
	mSubType = el.attribute( QLatin1String( "Subtype" ), QString() );
	mValidatorRule = el.attribute( QLatin1String( "Validate" ), QString() );
}

bool ConfigSetting::isSubSectioned() const
{
	return mSubSection != NULL;
}

QString ConfigSetting::name() const
{
	return mName;
}

QString ConfigSetting::fullName() const
{
	QString fn;

	if( mSubSection )
		fn = mSubSection->fullName() % mName;
	else
		fn = mName;

	//qDebug() << "CS::fn => " << fn;

	return fn;
}

QString ConfigSetting::fullPath() const
{
	if( mSubSection )
		return mSubSection->fullPath() % QChar( L'/' ) % mName;
	else
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

QString ConfigSetting::defaultValue() const
{
	return mDefaultValue;
}

QString ConfigSetting::defaultInitializer() const
{
	if( mType == QLatin1String( "String" ) )
	{
		return QLatin1String( "QString::fromUtf8( \"" )
				% utf8Encoded( mDefaultValue )
				% QLatin1String( "\" )" );
	}
	else if( mDefaultValue.isEmpty() )
	{
		return type().defaultCTored();
	}
	else
	{
		return mDefaultValue;
	}
}

ConfigSubSection::ConfigSubSection( QDomElement el, ConfigSubSection* parent )
{
	mParent = parent;
	mName = el.attribute( QLatin1String( "Name" ), QString() );

	//qDebug() << "Subsection:" << mName;

	QDomElement elChild = el.firstChildElement();
	while( elChild.isElement() )
	{
		if( elChild.tagName() == QLatin1String( "Setting" ) )
		{
			mSettings.append( new ConfigSetting( elChild, this ) );
		}
		else if( elChild.tagName() == QLatin1String( "SubSection" ) )
		{
			mSections.append( new ConfigSubSection( elChild, this ) );
		}
		elChild = elChild.nextSiblingElement();
	}
}

ConfigSubSection::~ConfigSubSection()
{
	qDeleteAll( mSections );
	qDeleteAll( mSettings );
}

QList< ConfigSubSection* > ConfigSubSection::sections() const
{
	return mSections;
}

QList< ConfigSetting* > ConfigSubSection::settings() const
{
	return mSettings;
}

QString ConfigSubSection::name() const
{
	return mName;
}

QString ConfigSubSection::fullName() const
{
	QString fn;
	if( mParent )
		fn = mParent->fullName() % mName;
	else
		fn = mName;

	//qDebug() << "CSS::fn = " << fn;

	return fn;
}

QString ConfigSubSection::fullPath() const
{
	if( mParent )
		return mParent->fullName() % QChar( L'/' ) % mName;
	else
		return mName;
}

void ConfigSubSection::addAllSettings( QList< ConfigSetting* >& settings) const
{
	settings += mSettings;

	foreach( ConfigSubSection* sub, mSections )
	{
		sub->addAllSettings( settings );
	}
}


ConfigSection::ConfigSection( QDomElement el )
{
	mClassName = el.attribute( QLatin1String( "Class" ) );
	mConfigPath = el.attribute( QLatin1String( "ConfigPath" ) );

	QDomElement elChild = el.firstChildElement();
	while( elChild.isElement() )
	{
		if( elChild.tagName() == QLatin1String( "Setting" ) )
		{
			mSettings.append( new ConfigSetting( elChild ) );
		}
		else if( elChild.tagName() == QLatin1String( "SubSection" ) )
		{
			mSections.append( new ConfigSubSection( elChild ) );
		}
		elChild = elChild.nextSiblingElement();
	}

}

ConfigSection::~ConfigSection()
{
	qDeleteAll( mSections );
	qDeleteAll( mSettings );
}

QList< ConfigSubSection* > ConfigSection::sections() const
{
	return mSections;
}

QList< ConfigSetting* > ConfigSection::settings() const
{
	return mSettings;
}

QList< ConfigSetting* > ConfigSection::allSettings() const
{
	QList< ConfigSetting* > settings = mSettings;

	foreach( ConfigSubSection* sub, mSections )
	{
		sub->addAllSettings( settings );
	}

	return settings;
}

QString ConfigSection::className() const
{
	return mClassName;
}

QString ConfigSection::configPath() const
{
	return mConfigPath;
}
