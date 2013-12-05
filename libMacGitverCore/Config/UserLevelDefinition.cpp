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

#include <QSet>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QDomDocument>
#include <QStringBuilder>

#include "UserLevelDefinition.h"

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Mode.hpp"

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
        if( e.tagName() == QLatin1String( "Enable" ) )
        {
            mList.append( EnableDisable( e.attribute( QLatin1String( "Name" ) ), true ) );
        }
        else if( e.tagName() == QLatin1String( "Disable" ) )
        {
            mList.append( EnableDisable( e.attribute( QLatin1String( "Name" ) ), false ) );
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

UserLevelMode::UserLevelMode( UserLevelDefinition* level, const QString& modeName )
    : mLevel( level )
    , mModeName( modeName )
    , mHeavenMode( NULL )
    , mIsLocking( false )
    , mIsUserSelectable( true )
{
}

UserLevelMode::~UserLevelMode()
{
}

QString UserLevelMode::heavenModeName() const
{
    Q_ASSERT( mLevel );
    return mLevel->name() % QChar( L'#' ) % mModeName;
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

bool UserLevelMode::isLockingMode() const
{
    return mIsLocking;
}

bool UserLevelMode::isUserSelectable() const
{
    return mIsUserSelectable;
}

UserLevelMode::Ptr UserLevelMode::read( UserLevelDefinition* level, const QDomElement& el )
{
    UserLevelMode::Ptr mode( new UserLevelMode( level, el.attribute( QLatin1String( "Name" ) ) ) );

    if( el.hasAttribute( QLatin1String( "Selectable" ) ) )
    {
        if( el.attribute( QLatin1String( "Selectable" ) ) == QLatin1String( "0" ) )
        {
            mode->mIsUserSelectable = false;
        }
    }

    if( el.hasAttribute( QLatin1String( "Locking" ) ) )
    {
        if( el.attribute( QLatin1String( "Locking" ) ) == QLatin1String( "1" ) )
        {
            mode->mIsLocking = true;
        }
    }

    for( QDomElement e = el.firstChildElement(); e.isElement(); e = e.nextSiblingElement() )
    {
        if( e.tagName() == QLatin1String( "Views" ) )
        {
            mode->mAllowedViews = EnableDisableList( e );
        }
        else if( e.tagName() == QLatin1String( "DefaultLayout" ) )
        {
            BlueSky::Mode* m = new BlueSky::Mode(mode->heavenModeName().toUtf8());
            m->readFromState(e, true);
            BlueSky::Application::instance()->addMode(m);
            mode->mHeavenMode = m;
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

    def->mName = el.attribute( QLatin1String( "name" ) );
    def->mId = el.attribute( QLatin1String( "id" ) );
    def->mAppLevel = el.attribute( QLatin1String( "applevel" ) ).toInt();
    def->mPrecedence = el.attribute( QLatin1String( "precedence" ) ).toInt();

    QString foo;
    QTextStream ts( &foo );
    el.firstChildElement( QLatin1String( "desc" ) ).save( ts, 0 );

    def->mDescription = foo.replace(
                            QLatin1String( "<desc" ),
                            QLatin1String( "<html" ) ).simplified();

    QDomElement e = el.firstChildElement( QLatin1String( "gui" ) );
    if( e.isElement() )
    {
        def->readGuiDef( e.attribute( QLatin1String( "file" ) ) );
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

    if( mHeavenPresets.contains( QLatin1String( "*" ) ) )
    {
        return mHeavenPresets[ QLatin1String( "*" ) ];
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
        if( el.tagName() == QLatin1String( "Mode" ) )
        {
            UserLevelMode::Ptr mode = UserLevelMode::read( this, el );
            if( !mode )
            {
                return;
            }
            mModes.append( mode );
        }
        else if( el.tagName() == QLatin1String( "Preset" ) )
        {
            QString preset = el.attribute( QLatin1String( "Name" ) );
            QString mode = el.attribute( QLatin1String( "Mode" ) );
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
