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
    return mType == QStringLiteral( "User" );
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
    return QString::fromUtf8(QVariant::typeToName(mTypeId));
}

QString VariantType::defaultCTored() const
{
    switch( mTypeId )
    {
    case QVariant::Bool:        return QStringLiteral( "false" );
    case QVariant::Int:
    case QVariant::UInt:        return QStringLiteral( "0" );
    case QVariant::LongLong:
    case QVariant::ULongLong:   return QStringLiteral( "0LL" );
    case QVariant::Double:      return QStringLiteral( "0.0" );
    case QVariant::Char:        return QStringLiteral( "'\\0'" );

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
    case QVariant::Hash:        return mCppType + QStringLiteral( "()" );
    default:                    return QStringLiteral( "/* UNSUPPORTED */" );
    }
}

VariantTypes::VariantTypes()
{
    mTypes.append(VariantType(QStringLiteral("String"),
                              QVariant::String,
                              QStringLiteral("QString")));

    mTypes.append(VariantType(QStringLiteral("Int"),
                              QVariant::Int,
                              QStringLiteral("qint32")));

    mTypes.append(VariantType(QStringLiteral("UInt"),
                              QVariant::UInt,
                              QStringLiteral("quint32")));

    mTypes.append(VariantType(QStringLiteral("Bool"),
                              QVariant::Bool,
                              QStringLiteral("bool")));
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
