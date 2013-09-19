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


RefItem::RefItem(RefItem *p)
    : parent( p )
{
    Q_ASSERT( p );
    p->children.append( this );
}

RefItem::~RefItem()
{
    if( parent )
    {
        parent->children.removeOne( this );
    }
    qDeleteAll( children );
}

bool RefItem::isContentItem() const
{
    return false;
}

QVariant RefItem::data(int col, int role) const
{
    Q_UNUSED( col )
    Q_UNUSED( role )
    return QVariant();
}

bool RefItem::setData(Git::Result& result, const QVariant &value, int role, int col)
{
    Q_UNUSED( value );
    Q_UNUSED( role );
    Q_UNUSED( col );
    return false;
}

QString RefItem::text() const
{
    return QString();
}

bool RefItem::isEditable() const
{
    return false;
}


RefScope::RefScope(RefItem *p, const QString &t)
    : RefItem( p )
    , mText ( t )
{
}

QVariant RefScope::data(int col, int role) const
{
    switch( role )
    {
    case Qt::DisplayRole:
        return mText;

    case Qt::BackgroundRole:
        QLinearGradient g( 0, 0, 100, 0 );
        g.setColorAt( 0.0, QColor(0, 0, 0, 0) );
        g.setColorAt( 1.0, QColor(216, 233, 255) );
        return QBrush( g );
    }

    return QVariant();
}

QString RefScope::text() const
{
    return mText;
}


RefNameSpace::RefNameSpace(RefItem *p, const QString &t)
    : RefScope( p, t )
{
}

QVariant RefNameSpace::data(int col, int role) const
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


RefBranch::RefBranch(RefItem *p, const QString &t, const Git::Reference &ref)
    : RefItem( p )
    , mRef( ref )
{
}

QVariant RefBranch::data(int col, int role) const
{
    if ( role == Qt::DisplayRole )
        return mRef.shorthand().section( QChar(L'/'), -1 );

    else if ( role == Qt::FontRole )
    {
        if( mRef.isCurrentBranch() )
        {
            QFont f;
            f.setBold( true );
            return f;
        }
    }

    else if ( role == Qt::BackgroundRole )
    {
        Git::Result r;
        if ( mRef.isCurrentBranch() )
        {
            QLinearGradient g( 0, 0, 0, 30 );
            g.setColorAt( 0.0, QColor(255, 255, 255, 0) );
            g.setColorAt( 0.5, QColor(255, 181, 79) );
            g.setColorAt( 1.0, QColor(255, 255, 255, 0) );
            return QBrush(g);
        }
        else if ( mRef.compare( mRef.repository(r).HEAD(r) ) == 0 )
        {
            QLinearGradient g( 0, 0, 0, 30 );
            g.setColorAt( 0.0, QColor(255, 255, 255, 0) );
            g.setColorAt( 0.5, QColor(255, 181, 79).lighter() );
            g.setColorAt( 1.0, QColor(255, 255, 255, 0) );
            return QBrush(g);
        }
    }

    else if ( role == Qt::EditRole)
        return mRef.name();

    return QVariant();
}

bool RefBranch::setData(Git::Result& result, const QVariant &value, int role, int col)
{
    if ( col == 0 )
    {
        QString newName = value.toString();
        if ( newName.isEmpty() || (newName == mRef.name()) )
            return false;

        mRef.rename( result, newName );
        return result;
    }

    return false;
}

bool RefBranch::isContentItem() const
{
    return true;
}

bool RefBranch::isEditable() const
{
    return true;
}
