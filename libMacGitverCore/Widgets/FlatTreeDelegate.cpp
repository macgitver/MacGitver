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

#include <QPainter>

#include "FlatTreeModelPrivate.h"
#include "FlatTreeDelegate.h"

FlatTreeDelegate::FlatTreeDelegate( QObject* parent )
    : QItemDelegate( parent )
{
}

void FlatTreeDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
                              const QModelIndex& index ) const
{
    const FlatTreeModel* model = qobject_cast< const FlatTreeModel* >( index.model() );
    if( !model )
    {
        QItemDelegate::paint( painter, option, index );
    }

    drawBackground( painter, option, index );

    QFont f = option.font;
    if( index.data( Qt::UserRole + 1 ).toBool() )
    {
        f.setBold( true );
    }

    int left = (index.data( Qt::UserRole + 2 ).toInt()-1) * 18;

    QRect rect = option.rect.adjusted( left, 0, 0, 0 );

    QIcon icon = index.data( Qt::DecorationRole ).value< QIcon >();
    if( !icon.isNull() )
    {
        QRect iconRect = rect;
        iconRect.setWidth( 16 );
        icon.paint( painter, iconRect, Qt::AlignVCenter );
        rect.adjust( 18, 0, 0, 0 );
    }

    rect.adjust( 1, 1, -1, -1 );
    painter->setFont( f );
    painter->drawText( rect, Qt::AlignLeft | Qt::AlignVCenter,
                       index.data( Qt::UserRole + 3 ).toString() );

    rect = option.rect.adjusted( left, 0, 0, 0 );
    drawFocus( painter, option, rect );
}

QSize FlatTreeDelegate::sizeHint( const QStyleOptionViewItem& option,
                                  const QModelIndex& index ) const
{
    const FlatTreeModel* model = qobject_cast< const FlatTreeModel* >( index.model() );
    if( !model )
    {
        return QItemDelegate::sizeHint( option, index );
    }

    QFont f = option.font;
    if( index.data( Qt::UserRole + 1 ).toBool() )
    {
        f.setBold( true );
    }
    QFontMetrics fm( f );

    int left = (index.data( Qt::UserRole + 2 ).toInt()-1) * 18;

    if( !index.data( Qt::DecorationRole ).value< QIcon >().isNull() )
    {
        left += 18;
    }

    QString s = index.data( Qt::UserRole + 3 ).toString();
    return QSize( left + fm.width( s ), qMax( 18, fm.lineSpacing() + 2 ) );
}
