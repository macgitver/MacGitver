/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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
#include <QSettings>
#include <QApplication>
#include <QFontInfo>

#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/ConfigUser.h"

Config::Config()
    : mSettings( NULL )
{
}

Config::~Config()
{
    // QSettings' DTor will sync()
    delete mSettings;
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

void Config::loadSettings()
{
    delete mSettings;
    mSettings = new QSettings();


    QString fontName = get( "General/Font", "#" ).toString();
    if( fontName == QLatin1String( "#" ) )
        mDefaultFont = QFont();
    else
        mDefaultFont.fromString( fontName );

    fontName = get( "General/DialogFont", "#" ).toString();
    if( fontName == QLatin1String( "#" ) )
        mDefaultDialogFont = mDefaultFont;
    else
        mDefaultDialogFont.fromString( fontName );

    fontName = get( "General/FixedFont", "#" ).toString();
    if( fontName == QLatin1String( "#" ) )
        mDefaultFixedFont = QFont( QLatin1String( "Courier New" ), 10 );
    else
        mDefaultFixedFont.fromString( fontName );
}

void Config::saveSettings()
{
    if( mSettings )
    {
        mSettings->sync();
    }
}

QVariant Config::get( const char* szPath, const char* szDefaultValue ) const
{
    return get( szPath, QLatin1String( szDefaultValue ) );
}

QVariant Config::get( const char* szPath, const QVariant& defaultValue ) const
{
    return get( QLatin1String( szPath ), defaultValue );
}

QVariant Config::get( const QString& path, const QVariant& defaultValue ) const
{
    if( mSettings )
    {
        return mSettings->value( path, defaultValue );
    }

    const_cast< Config* >( this )->loadSettings();

    if( mSettings )
    {
        return mSettings->value( path, defaultValue );
    }

    return defaultValue;
}

void Config::set( const char* pszPath, const QVariant& value )
{
    set( QLatin1String( pszPath ), value );
}

void Config::set( const QString& path, const QVariant& value )
{
    if( !mSettings )
    {
        loadSettings();
    }
    Q_ASSERT( mSettings );

    mSettings->setValue( path, value );

    foreach( ConfigUser* user, mConfigUsers )
    {
        if( path.startsWith( user->configBasePath() ) )
        {
            QString subPath = path.mid( user->configBasePath().length() + 1 );
            user->configChanged( subPath, value );
        }
    }
}


QFont Config::defaultFont()
{
    return self().mDefaultFont;
}

QFont Config::defaultDialogFont()
{
    return self().mDefaultDialogFont;
}

QFont Config::defaultFixedFont()
{
    return self().mDefaultFixedFont;
}

void Config::setDefaultFont( const QFont& font )
{
    mDefaultFont = font;

    set( "General/Font", font.toString() );

    emit fontsChanged();
}

void Config::setDefaultDialogFont( const QFont& font )
{
    mDefaultDialogFont = font;

    set( "General/DialogFont", font.toString() );

    emit fontsChanged();
}

void Config::setDefaultFixedFont( const QFont& font )
{
    mDefaultFixedFont = font;

    set( "General/FixedFont", font.toString() );

    emit fontsChanged();
}

void Config::addConfigUser( ConfigUser* user )
{
    Q_ASSERT( !mConfigUsers.contains( user ) );
    mConfigUsers.insert( user );
}

void Config::delConfigUser( ConfigUser* user )
{
    Q_ASSERT( mConfigUsers.contains( user ) );
    mConfigUsers.remove( user );
}

/**
 * @internal
 * @brief       Convert a QFont to a string that can be embedded into CSS.
 *
 * @param[in]   font    The font to convert to.
 *
 * @return      A QString that can be embedded into CSS.
 *
 * This method considers font family and size. Weight and style and anything
 * else is simply ignored.
 *
 */
QString Config::fontCSS(const QFont& font)
{
    QFontInfo info = QFontInfo(font);
    return QString::fromUtf8("font-family: \"%1\"; font-size: %2px;")
            .arg(font.family())
            .arg(info.pixelSize());
}

/**
 * @brief       Get the default font as string embeddable into CSS.
 *
 * @return      A QString that can be embedded into CSS.
 *
 * This method considers font family and size. Weight and style and anything
 * else is simply ignored.
 *
 */
QString Config::defaultFontCSS()
{
    return fontCSS(defaultFont());
}

/**
 * @brief       Get the default fixed font as string embeddable into CSS.
 *
 * @return      A QString that can be embedded into CSS.
 *
 * This method considers font family and size. Weight and style and anything
 * else is simply ignored.
 *
 */
QString Config::defaultFixedFontCSS()
{
    return fontCSS(defaultFixedFont());
}
