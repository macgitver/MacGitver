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

#include "VariantTypes.hpp"

VariantType::VariantType()
{
}

VariantType::VariantType( const QString& typeName,
                          QVariant::Type typeId,
                          const QString& cppType,
                          bool templated )
    : mType( typeName )
    , mSubType()
    , mCppType( cppType )
    , mTypeId( typeId )
    , mTemplated( templated )
{
}

VariantType::VariantType( const QString& typeName,
                          const QString& subType,
                          QVariant::Type typeId,
                          const QString& cppType,
                          bool templated )
    : mType( typeName )
    , mSubType( subType )
    , mCppType( cppType )
    , mTypeId( typeId )
    , mTemplated( templated )
{
}

bool VariantType::isUserType() const
{
    return mType == QLatin1String( "User" );
}

bool VariantType::requiresTemplateMagic() const
{
    return mTemplated || isUserType();
}

QString VariantType::cppType() const
{
    return mCppType;
}

QString VariantType::type() const
{
    return mType;
}

QString VariantType::subType() const
{
    return mSubType;
}

QVariant::Type VariantType::typeId() const
{
    return mTypeId;
}

QString VariantType::typeIdName() const
{
    return QLatin1String( QVariant::typeToName( mTypeId ) );
}

QString VariantType::defaultCTored() const
{
    switch( mTypeId )
    {
    case QVariant::Bool:        return QLatin1String( "false" );
    case QVariant::Int:
    case QVariant::UInt:        return QLatin1String( "0" );
    case QVariant::LongLong:
    case QVariant::ULongLong:   return QLatin1String( "0LL" );
    case QVariant::Double:      return QLatin1String( "0.0" );
    case QVariant::Char:        return QLatin1String( "'\\0'" );

    case QVariant::Map:
    case QVariant::List:
    case QVariant::String:
    case QVariant::StringList:
    case QVariant::ByteArray:
    case QVariant::BitArray:
    case QVariant::Date:
    case QVariant::Time:
    case QVariant::DateTime:
    case QVariant::Url:
    case QVariant::Locale:
    case QVariant::Rect:
    case QVariant::RectF:
    case QVariant::Size:
    case QVariant::SizeF:
    case QVariant::Line:
    case QVariant::LineF:
    case QVariant::Point:
    case QVariant::PointF:
    case QVariant::RegExp:
    case QVariant::Hash:        return mCppType + QLatin1String( "()" );
    default:                    return QLatin1String( "/* UNSUPPORTED */" );
    }
}

VariantTypes::VariantTypes()
{
    mTypes.append( VariantType( QLatin1String( "String" ),
                                QVariant::String,
                                QLatin1String( "QString" ) ) );

    mTypes.append( VariantType( QLatin1String( "Int" ),
                                QVariant::Int,
                                QLatin1String( "qint32" ) ) );

    mTypes.append( VariantType( QLatin1String( "UInt" ),
                                QVariant::UInt,
                                QLatin1String( "quint32" ) ) );
}

VariantTypes* VariantTypes::sSelf = NULL;

VariantTypes& VariantTypes::self()
{
    if( !sSelf )
    {
        sSelf = new VariantTypes;
    }

    return *sSelf;
}

VariantType VariantTypes::typeFor( const QString& type, const QString& subType )
{
    for( int i = 0; i < mTypes.count(); i++ )
    {
        if( mTypes[ i ].type() == type && mTypes[ i ].subType() == subType )
        {
            return mTypes[ i ];
        }
    }

    return VariantType();
}
