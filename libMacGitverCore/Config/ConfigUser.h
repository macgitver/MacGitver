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

#ifndef MGV_CONFIG_USER_H
#define MGV_CONFIG_USER_H

#include <QVariant>

#include "libMacGitverCore/MacGitverApi.hpp"

class MGV_CORE_API ConfigUser
{
protected:
    ConfigUser( const QString& configBasePath );
    ConfigUser( const char* pszConfigBasePath );
    virtual ~ConfigUser();

public:
    QVariant configGetV( const QString& subPath, const QVariant& defaultValue = QVariant() ) const;
    QVariant configGetV( const char* pszSubPath, const QVariant& defaultValue = QVariant() ) const;

    void configSet( const QString& subPath, const QVariant& value ) const;
    void configSet( const char* pszsubPath, const QVariant& value ) const;

    QString configSubPath( const QString& subPath ) const;
    QString configSubPath( const char* pszSubPath ) const;

    QString configBasePath() const;

    template< class T >
    inline T configGet( const char* pszSubPath, const T& t ) const
    {
        QVariant vT; vT = t;
        return configGetV( pszSubPath, vT ).value< T >();
    }

    template< class T >
    inline T configGet( const QString& subPath, const T& t ) const
    {
        QVariant vT; vT = t;
        return configGetV( subPath, vT ).value< T >();
    }

    inline QString configGet( const char* pszSubPath, const char* pszValue ) const
    {
        return configGet< QString >( pszSubPath, QLatin1String( pszValue ) );
    }

public:
    virtual void configChanged( const QString& subPath, const QVariant& value );

private:
    QString         mConfigBase;
};

#endif
