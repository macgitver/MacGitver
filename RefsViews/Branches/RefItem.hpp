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

#pragma once

#include "libGitWrap/Reference.hpp"

#include <QList>
#include <QVariant>

namespace RM
{
    class Ref;
}


class RefItem
{
public:
    enum Role
    {
        RowBgRole = Qt::UserRole + 1,
        RowBgGradientRole = Qt::UserRole + 2
    };

    enum ItemType
    {
        Root = 1,
        Scope,
        Namespace,
        Reference,
        Branch = Reference
    };

public:
    RefItem();
    RefItem( RefItem* p );
    virtual ~RefItem();

public:
    virtual bool isValid() const;
    virtual ItemType type() const = 0;
    virtual bool sameReference(const RM::Ref* ref) const { return false; }

public:
    RefItem* parent;
    QList< RefItem* > children;

    virtual QVariant data( int col, int role ) const;
    virtual QString text() const;
};

class RefRoot : public RefItem
{
public:
    RefRoot() {}

public:
    ItemType type() const;
};

class RefScope : public RefItem
{
public:
    RefScope( RefItem* p, const QString& t );

protected:
    virtual QVariant data( int col, int role ) const;
    QString text() const;
    ItemType type() const;

public:
    QString mText;
};


class RefNameSpace : public RefScope
{
public:
    RefNameSpace( RefItem* p, const QString& t );

    QVariant data( int col, int role ) const;
    ItemType type() const;
};


class RefBranch : public RefItem
{
public:
    explicit RefBranch(RefItem* p, const Git::Reference &ref);

public:
    bool isValid() const;
    bool sameReference(const RM::Ref* ref) const;

    QVariant data( int col, int role ) const;
    ItemType type() const;

    Git::Reference reference() const
    {
        return mRef;
    }

private:
    Git::Reference  mRef;
};
