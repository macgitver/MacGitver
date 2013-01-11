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

#ifndef CFGCOMP_VARIANT_TYPES_HPP
#define CFGCOMP_VARIANT_TYPES_HPP

#include <QVariant>
#include <QString>
#include <QList>

class VariantType
{
public:
    VariantType();

    VariantType( const QString& typeName,
                 QVariant::Type typeId,
                 const QString& cppType,
                 bool templated = false );

    VariantType( const QString& typeName,
                 const QString& subType,
                 QVariant::Type typeId,
                 const QString& cppType,
                 bool templated = false );

public:
    bool isUserType() const;
    bool requiresTemplateMagic() const;

    QString cppType() const;
    QString type() const;
    QString subType() const;
    QVariant::Type typeId() const;
    QString typeIdName() const;

    QString defaultCTored() const;

private:
    QString         mType;
    QString         mSubType;
    QString         mCppType;
    QVariant::Type  mTypeId;
    bool            mTemplated;
};

class VariantTypes
{
public:
    static VariantTypes& self();

    VariantType typeFor( const QString& type, const QString& subType = QString() );

private:
    VariantTypes();
    static VariantTypes* sSelf;
    QList< VariantType > mTypes;
};

#endif
