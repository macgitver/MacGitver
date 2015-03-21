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

#include "SeqViewContainer.hpp"

namespace DiffViews
{

    SeqViewContainer::SeqViewContainer( SeqViewInfo* info )
        : SeqViewItem( info )
    {
        mMarginTop = 0.;
        mMarginBottom = 0.;
    }

    qreal SeqViewContainer::setWidth( qreal width )
    {
        qreal height = mMarginTop;

        QList< QGraphicsItem* > items = childItems();
        foreach( QGraphicsItem* itGI, items )
        {
            SeqViewItem* it = static_cast< SeqViewItem* >( itGI );
            QPointF posChild( 0, height );
            it->setPos( posChild );
            height += it->setWidth( width );
        }

        height += mMarginBottom;

        SeqViewItem::setWidth( width, height );
        return height;
    }

    void SeqViewContainer::postRendering()
    {
        QList< QGraphicsItem* > items = childItems();
        foreach( QGraphicsItem* itGI, items )
        {
            SeqViewItem* it = static_cast< SeqViewItem* >( itGI );
            it->postRendering();
        }
    }

    void SeqViewContainer::setVerticalMargins( qreal top, qreal bottom )
    {
        mMarginTop = top;
        mMarginBottom = bottom;
    }

    void SeqViewContainer::paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* )
    {
        /* nothing */
    }

}
