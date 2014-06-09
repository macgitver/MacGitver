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

#include <QDebug>
#include <QTextStream>
#include <QPainter>

#include "SequentialView.hpp"
#include "SeqViewHunk.hpp"

namespace DiffViews
{

    SeqViewHunkHeader::SeqViewHunkHeader( SeqViewInfo* info, Hunk::Ptr hunk )
        : SeqViewItem( info )
        , mText( hunk->completeHeader() )
    {
    }

    qreal SeqViewHunkHeader::setWidth( qreal width )
    {
        QFontMetricsF fm( info()->mFixed );
        qreal height = 2 + fm.lineSpacing();

        SeqViewItem::setWidth( width, height );
        return height;
    }

    void SeqViewHunkHeader::paint( QPainter* p, const QStyleOptionGraphicsItem*, QWidget* )
    {
        SeqViewInfo* i = info();
        QPen pen(i->clrSeparator);
        pen.setWidth(0);

        QFontMetricsF fm( i->mFixed );

        p->setPen(pen);
        p->setBrush( i->clrDeltaFirst );
        p->drawRect( 10., 0., width() - 20., height() );

        pen.setColor(i->clrText);
        p->setPen(pen);
        p->setFont( i->mFixed );
        p->drawText( QRectF( 12, 1 - fm.leading() / 2., width() - 24, height() - 2 ),
                     Qt::AlignLeft | Qt::AlignTop | Qt::TextWrapAnywhere, mText );
    }

    SeqViewHunkContent::SeqViewHunkContent( SeqViewInfo* info, Hunk::Ptr hunk )
        : SeqViewItem( info )
    {
        #if 0
        QString s; QTextStream ts( &s ); hunk->exportRaw( ts );
        qDebug() << s;
        #endif

        int l = hunk->firstLine( 0 );
        int r = hunk->firstLine( 1 );
        foreach( HunkPart::Ptr part, hunk->parts() )
        {
            Line line;
            switch( part->type() )
            {
            case HunkPart::Context:
                for( int i = 0; i < part->numLines( 0 ); ++i )
                {
                    line.leftNr = l++;
                    line.rightNr = r++;
                    line.text = part->sideLines( 0 )->lineAt( i );
                    mLines.append( line );
                }
                break;

            case HunkPart::Insert:
                for( int i = 0; i < part->numLines( 1 ); ++i )
                {
                    line.leftNr = 0;
                    line.rightNr = r++;
                    line.text = part->sideLines( 1 )->lineAt( i );
                    mLines.append( line );
                }
                break;

            case HunkPart::Delete:
                for( int i = 0; i < part->numLines( 0 ); ++i )
                {
                    line.leftNr = l++;
                    line.rightNr = 0;
                    line.text = part->sideLines( 0 )->lineAt( i );
                    mLines.append( line );
                }
                break;

            case HunkPart::Change:
                for( int i = 0; i < part->numLines( 0 ); ++i )
                {
                    line.leftNr = l++;
                    line.rightNr = 0;
                    line.text = part->sideLines( 0 )->lineAt( i );
                    mLines.append( line );
                }
                for( int i = 0; i < part->numLines( 1 ); ++i )
                {
                    line.leftNr = 0;
                    line.rightNr = r++;
                    line.text = part->sideLines( 1 )->lineAt( i );
                    mLines.append( line );
                }
                break;
            }
        }
    }

    qreal SeqViewHunkContent::setWidth( qreal width )
    {
        QFontMetricsF fm( info()->mFixed );
        QRectF r = fm.boundingRect( QLatin1String( "X" ) );
        qreal height = 1 + ( r.height() + 1 ) * mLines.count();

        mSpaceLeft = mSpaceRight = 20;
        for( int i = 0; i < mLines.count(); ++i )
        {
            if( mLines[ i ].leftNr )
            {
                qreal space = fm.width( QString::number( mLines[ i ].leftNr ) );
                mSpaceLeft = qMax( mSpaceLeft, space );
            }

            if( mLines[ i ].rightNr )
            {
                qreal space = fm.width( QString::number( mLines[ i ].rightNr ) );
                mSpaceRight = qMax( mSpaceRight, space );
            }
        }

        SeqViewItem::setWidth( width, height );
        return height;
    }

    void SeqViewHunkContent::paint( QPainter* p, const QStyleOptionGraphicsItem*, QWidget* )
    {
        SeqViewInfo* ifo = info();

        QFontMetricsF fm( ifo->mFixed );
        QRectF outline(10, 0, width() - 20, height());
        QRectF r = fm.boundingRect( QLatin1String( "X" ) );
        qreal lh = r.height() + 1;
        qreal top = 0;

        qreal left = 12 + mSpaceLeft + mSpaceRight + 6;
        qreal wide = width() - 12 - left;

        p->fillRect(outline, ifo->clrDeltaFirst);
        p->setFont( ifo->mFixed );

        QPen pen(ifo->clrText);
        pen.setWidth(0);
        p->setPen(pen);

        for( int i = 0; i < mLines.count(); ++i )
        {
            if( !mLines[ i ].rightNr )
            {
                p->fillRect( QRectF( 10, top, width() - 20, lh ), ifo->clrRemoved );
            }
            else if( !mLines[ i ].leftNr )
            {
                p->fillRect( QRectF( 10, top, width() - 20, lh ), ifo->clrAdded );
            }

            qreal top2 = top - fm.leading() / 2.;
            if( mLines[ i ].leftNr )
            {
                p->drawText( QRectF( 12, top2, mSpaceLeft, lh ),
                             Qt::AlignRight | Qt::AlignTop | Qt::TextWrapAnywhere,
                             QString::number( mLines[ i ].leftNr ) );
            }

            if( mLines[ i ].rightNr )
            {
                p->drawText( QRectF( 12 + mSpaceLeft + 2, top2, mSpaceRight, lh ),
                             Qt::AlignRight | Qt::AlignTop | Qt::TextWrapAnywhere,
                             QString::number( mLines[ i ].rightNr ) );
            }

            p->drawText( QRectF( left, top2, wide, lh ),
                         Qt::AlignLeft | Qt::AlignTop | Qt::TextWrapAnywhere,
                         mLines[ i ].text );

            top += lh;
        }

        pen.setColor(ifo->clrSeparator);
        p->setPen(pen);
        p->drawRect(outline);
        p->drawLine( 12 + mSpaceLeft + 1, 0,
                     12 + mSpaceLeft + 1, height() );

        p->drawLine( 12 + mSpaceLeft + mSpaceRight + 2, 0,
                     12 + mSpaceLeft + mSpaceRight + 2, height() );
    }

}
