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

#include "SeqViewItem.hpp"

namespace DiffViews
{

    SeqViewItem::SeqViewItem( SeqViewInfo* info )
        : mInfo( info )
        , mWidth( .0 )
        , mHeight( .0 )
    {
    }

    QRectF SeqViewItem::boundingRect() const
    {
        return QRectF( 0., 0., mWidth, mHeight );
    }

    void SeqViewItem::setWidth( qreal width, qreal height )
    {
        // fuzzy compare?
        if( width != mWidth || height != mHeight )
        {
            prepareGeometryChange();
            mWidth = width;
            mHeight = height;
        }
    }

    void SeqViewItem::postRendering()
    {
    }

    qreal SeqViewItem::width() const
    {
        return mWidth;
    }

    qreal SeqViewItem::height() const
    {
        return mHeight;
    }

    SeqViewInfo* SeqViewItem::info() const
    {
        return mInfo;
    }

}
