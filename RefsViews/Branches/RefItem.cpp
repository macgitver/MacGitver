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

#include "RefItem.hpp"
#include "BranchesModel.hpp"

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/Result.hpp"

#include <QFont>
#include <QLinearGradient>
#include <QFileIconProvider>

RefItem::RefItem()
    : parent( NULL )
{
}

RefItem::RefItem(RefItem* parent)
    : parent(parent)
{
    Q_ASSERT(parent);
}

RefItem::~RefItem()
{
    if( parent )
    {
        parent->children.removeOne( this );
    }
    qDeleteAll( children );
}

RM::Base* RefItem::object()
{
    return NULL;
}

int RefItem::itemPosition()
{
    int i = 0;
    while (i < parent->children.count()) {
        RefItem* sibling = parent->children.at(i);
        if (sibling->data(Qt::DisplayRole).toString() > data(Qt::DisplayRole).toString()) {
            break;
        }
        i++;
    }

    return i;
}

QVariant RefItem::data(int role) const
{
    Q_UNUSED( role )
    return QVariant();
}

int RefHeadline::itemPosition()
{
    int i = parent->children.indexOf(this);
    return (i == -1) ? parent->children.count() : i;
}

QVariant RefHeadline::data(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return mText;

    case RefItem::RowBgRole:
        return QColor(216, 233, 255);

    default:
        return QVariant();
    }
}

RefItem::ItemType RefHeadline::type() const
{
    return Headline;
}

RefItem::ItemType RefScope::type() const
{
    return Scope;
}

#if 0
RefNameSpace::RefNameSpace(RefItem *p, const QString &t)
    : RefScope( p, t )
{
}

RefItem::ItemType RefNameSpace::type() const
{
    return Namespace;
}

QVariant RefNameSpace::data(int role) const
{
    switch( role )
    {
    case Qt::DisplayRole:
        return mText;

    case Qt::DecorationRole:
        return QFileIconProvider().icon( QFileIconProvider::Folder );
    }

    return QVariant();
}
#endif

RefItem::ItemType RefBranch::type() const
{
    return Branch;
}

QVariant RefBranch::data(int role) const
{
    Git::Result r;
    Q_ASSERT(mObject);

    switch (role) {
    case Qt::FontRole:
        #if 0
        if( mRef.isCurrentBranch() )
        {
            QFont f;
            f.setBold( true );
            return f;
        }
        #endif
        break;

    case RefItem::RowBgGradientRole:
        #if 0
        if ( mRef.compare( mRef.repository().HEAD(r) ) == 0 )
        {
            QColor back = mRef.isCurrentBranch()
                          ? QColor::fromHsl(35, 255, 190)
                          : QColor::fromHsl(35, 255, 190).lighter(130);
            return back;
        }
        #endif
        break;
    }

    return RefItemObject::data(role);
}

RefItem::ItemType RefTag::type() const
{
    return Tag;
}

QVariant RefTag::data(int role) const
{
    Git::Result r;
    Q_ASSERT(mObject);

    switch (role) {
    case Qt::EditRole:
        return object()->name();
    }

    return RefItemObject::data(role);
}

RefItem::ItemType RefRoot::type() const
{
    return Root;
}


RefItem::ItemType RefRemote::type() const
{
    return Remote;
}
