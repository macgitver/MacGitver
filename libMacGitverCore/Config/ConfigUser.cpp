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

#include <QStringBuilder>

#include "Config.h"
#include "ConfigUser.h"


ConfigUser::ConfigUser( const QString& configBasePath )
    : mConfigBase( configBasePath )
{
    Config::self().addConfigUser( this );
}

ConfigUser::ConfigUser( const char* pszConfigBasePath )
    : mConfigBase( QLatin1String( pszConfigBasePath ) )
{
    Config::self().addConfigUser( this );
}

ConfigUser::~ConfigUser()
{
    Config::self().delConfigUser( this );
}

QString ConfigUser::configSubPath( const QString& subPath ) const
{
    return mConfigBase % QChar( L'/' ) % subPath;
}

QString ConfigUser::configSubPath( const char* pszSubPath ) const
{
    return mConfigBase % QChar( L'/' ) % QLatin1String( pszSubPath );
}

QVariant ConfigUser::configGetV( const QString& subPath,
                                 const QVariant& defaultValue ) const
{
    return Config::self().get( configSubPath( subPath ), defaultValue );
}

QVariant ConfigUser::configGetV( const char* pszSubPath,
                                 const QVariant& defaultValue ) const
{
    return Config::self().get( configSubPath( pszSubPath ), defaultValue );
}

void ConfigUser::configSet( const QString& subPath, const QVariant& value ) const
{
    Config::self().set( configSubPath( subPath ), value );
}

void ConfigUser::configSet( const char* pszSubPath, const QVariant& value ) const
{
    Config::self().set( configSubPath( pszSubPath ), value );
}

QString ConfigUser::configBasePath() const
{
    return mConfigBase;
}

void ConfigUser::configChanged( const QString& subPath, const QVariant& value )
{
    Q_UNUSED( subPath );
    Q_UNUSED( value );
}
