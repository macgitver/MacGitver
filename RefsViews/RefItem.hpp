/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#ifndef REF_ITEM_HPP
#define REF_ITEM_HPP

#include "libGitWrap/Reference.hpp"

#include <QList>
#include <QVariant>


class RefItem
{
public:
    enum Role
    {
        TypeRole = Qt::UserRole,
        RowBgRole = Qt::UserRole + 1,
        RowBgGradientRole = Qt::UserRole + 2
    };

    enum ItemType
    {
        Scope = 1,
        Namespace,
        Reference
    };

public:
    RefItem();
    RefItem( RefItem* p );
    virtual ~RefItem();

public:
    RefItem* parent;
    QList< RefItem* > children;

    virtual QVariant data( int col, int role ) const;
    virtual bool setData(Git::Result &result, const QVariant &value, int role, int col );
    virtual QString text() const;

    virtual bool isEditable() const;
};


class RefScope : public RefItem
{
public:
    RefScope( RefItem* p, const QString& t );

protected:
    virtual QVariant data( int col, int role ) const;
    QString text() const;

public:
    QString mText;
};


class RefNameSpace : public RefScope
{
public:
    RefNameSpace( RefItem* p, const QString& t );

    QVariant data( int col, int role ) const;
};


class RefBranch : public RefItem
{
public:
    RefBranch( RefItem* p, const QString& t, const Git::Reference &ref );

    QVariant data( int col, int role ) const;
    bool setData(Git::Result& result, const QVariant &value, int role, int col);

    Git::Reference reference() const
    {
        return mRef;
    }

private:
    Git::Reference  mRef;
};

#endif // REF_ITEM_HPP
