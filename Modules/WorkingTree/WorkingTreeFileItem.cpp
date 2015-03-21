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

#include <QVariant>
#include <QColor>

#include "WorkingTreeFileItem.h"

WorkingTreeFileItem::WorkingTreeFileItem( WorkingTreeModel* model, WorkingTreeAbstractItem* parent )
    : WorkingTreeAbstractItem( model, parent )
    , mSize( 0 )
    , mState( Git::FileInvalidStatus )
{
}

WorkingTreeFileItem::~WorkingTreeFileItem()
{
}

int WorkingTreeFileItem::childCount() const
{
    return 0;
}

WorkingTreeAbstractItem* WorkingTreeFileItem::childAt( int index )
{
    Q_UNUSED( index );
    return NULL;
}

QVariant WorkingTreeFileItem::data( int column, int role ) const
{
    switch( role )
    {
    case Qt::DisplayRole:
        switch( column )
        {
        case 0:     return mName;
        case 1:     return mSize;
        case 2:     return mLastMod;
        case 3:     return mOwner;
        }
        break;

    case Qt::DecorationRole:
        if( column == 0 )
            return mIcon;
        break;

    case Qt::ForegroundRole:
        if( mState & Git::FileUnchanged )                       return QColor( Qt::black );
        else if( mState & Git::FileWorkingTreeModified )        return QColor( Qt::blue );
        else if( mState & Git::FileWorkingTreeNew )             return QColor( Qt::darkGreen );
        else if( mState & Git::FileWorkingTreeDeleted )         return QColor( Qt::red );
        else if( mState & Git::FileIgnored )                    return QColor( Qt::gray );
        else                                                    return QColor( 0xFFCCFF );

    case WorkingTreeAbstractItem::StatusRole:
        return int( mState );

    default:
        break;
    }

    return QVariant();
}

WorkingTreeAbstractItem* WorkingTreeFileItem::parent()
{
    return mParent;
}

int WorkingTreeFileItem::row() const
{
    return 0;
}

WorkingTreeAbstractItem* WorkingTreeFileItem::childByName( const QString& name )
{
    return NULL;
}

bool WorkingTreeFileItem::isDirectory() const
{
    return false;
}

void WorkingTreeFileItem::setName( const QString& name )
{
    mName = name;
}

void WorkingTreeFileItem::setState( Git::StatusFlags state )
{
    mState = state;
}

void WorkingTreeFileItem::setIcon( const QIcon& icon )
{
    mIcon = icon;
}

void WorkingTreeFileItem::setSize( qint64 size )
{
    mSize = size;
}

void WorkingTreeFileItem::setOwner( const QString& owner )
{
    mOwner = owner;
}

void WorkingTreeFileItem::setLastModified( const QDateTime& lastMod )
{
    mLastMod = lastMod;
}

QString WorkingTreeFileItem::name() const
{
    return mName;
}

void WorkingTreeFileItem::removeChild( WorkingTreeAbstractItem* child )
{
    Q_UNUSED( child );
}
