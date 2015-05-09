/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

    mName = el.attribute( QStringLiteral( "Name" ) );
    mDefaultValue = el.attribute( QStringLiteral( "Default" ), QString() );
    mType = el.attribute( QStringLiteral( "Type" ) );
    mSubType = el.attribute( QStringLiteral( "Subtype" ), QString() );
    mValidatorRule = el.attribute( QStringLiteral( "Validate" ), QString() );

    mEmitSignal = el.attribute( QStringLiteral( "Notify" ), QStringLiteral( "no" ) ).toLower() !=
            QStringLiteral( "no" );
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

bool ConfigSetting::emitSignal() const
{
    return mEmitSignal;
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
    if( mType == QStringLiteral( "String" ) )
    {
        return QStringLiteral( "QString::fromUtf8( \"" )
                % utf8Encoded( mDefaultValue )
                % QStringLiteral( "\" )" );
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
    mName = el.attribute( QStringLiteral( "Name" ), QString() );

    //qDebug() << "Subsection:" << mName;

    QDomElement elChild = el.firstChildElement();
    while( elChild.isElement() )
    {
        if( elChild.tagName() == QStringLiteral( "Setting" ) )
        {
            mSettings.append( new ConfigSetting( elChild, this ) );
        }
        else if( elChild.tagName() == QStringLiteral( "SubSection" ) )
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
    mClassName = el.attribute( QStringLiteral( "Class" ) );
    mConfigPath = el.attribute( QStringLiteral( "ConfigPath" ) );

    QDomElement elChild = el.firstChildElement();
    while( elChild.isElement() )
    {
        if( elChild.tagName() == QStringLiteral( "Setting" ) )
        {
            mSettings.append( new ConfigSetting( elChild ) );
        }
        else if( elChild.tagName() == QStringLiteral( "SubSection" ) )
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
