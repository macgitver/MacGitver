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
#include <QPainter>
#include <QPaintEvent>

#include "TabBarPrivate.h"

namespace Heaven
{

	static inline bool isHorizontal( TabBar::TabPos tabPos )
	{
		return tabPos == TabBar::North || tabPos == TabBar::South;
	}

	TabBar::Private::Private()
	{
		mMouseState = msNone;
		mTabPos = North;
		mCurrent = -1;
	}

	bool TabBar::Private::event( QEvent* ev )
	{
		switch( ev->type() )
		{
		case QEvent::Resize:
			calculate();
			return false;

		case QEvent::Paint:
			paint();
			return false;

		case QEvent::MouseButtonPress:
			return mousePress( static_cast< QMouseEvent* >( ev ) );

		case QEvent::MouseButtonRelease:
			return mouseRelease( static_cast< QMouseEvent* >( ev ) );

		case QEvent::MouseMove:
			return mouseMove( static_cast< QMouseEvent* >( ev ) );

		case QEvent::Leave:
			return mouseLeave();

		default:
			return false;
		}
	}


	bool TabBar::Private::mousePress( QMouseEvent* ev )
	{
		if( ev->button() == Qt::LeftButton && mMouseState == msNone )
		{
			mMouseState = msDown;
			mHovered = -1;
			for( int i = 0; i < mInfos.count(); i++ )
			{
				if( mInfos[ i ].mRect.contains( ev->pos() ) )
				{
					mHovered = i;
					break;
				}
			}
			mWidget->update();

			return true;
		}

		return false;
	}

	bool TabBar::Private::mouseRelease( QMouseEvent* ev )
	{
		if( ev->button() == Qt::LeftButton && mMouseState == msDown )
		{
			if( mHovered != -1 )
			{
				mWidget->setCurrentIndex( mHovered );
			}

			mMouseState = msNone;
			return true;
		}

		return false;
	}

	bool TabBar::Private::mouseMove( QMouseEvent* ev )
	{
		Q_UNUSED( ev );
		return false;
	}

	bool TabBar::Private::mouseLeave()
	{
		if( mMouseState != msNone )
		{
			mMouseState = msNone;
			mWidget->update();
		}

		return false;
	}

	void TabBar::Private::calculate()
	{
		QFont font1 = mWidget->font();
		QFont font2 = font1;
		font2.setBold( true );

		QFontMetrics fm1( font1 );
		QFontMetrics fm2( font2 );

		int textHeight = qMax( fm1.lineSpacing(), fm2.lineSpacing() ) + 2;

		int minWidth = 0;
		int left = 2;
		int totWidth = 0;
		bool horz = isHorizontal( mTabPos );

		for( int i = 0; i < mInfos.count(); i++ )
		{
			int width = ( i == mCurrent ) ?
							fm2.width( mInfos[ i ].mName ) :
							fm1.width( mInfos[ i ].mName );

			if( horz )
			{
				mInfos[ i ].mRect = QRect( left + 3, 3, width + 10, textHeight );
				left = mInfos[ i ].mRect.right() + 5;
			}
			else
			{
				mInfos[ i ].mRect = QRect( 3, left + 3, textHeight, width + 10 );
				left = mInfos[ i ].mRect.bottom() + 5;
			}

			minWidth = qMax( minWidth, width );
			totWidth = totWidth + width + 4 + 5;
		}

		QSize ms = horz ? QSize( minWidth, textHeight + 7 ) :
						  QSize( textHeight + 7, minWidth );
		if( ms != mMinSize )
		{
			mMinSize = ms;
			qDebug() << "MS=" << ms;
			mWidget->updateGeometry();
		}

		QSize sh = horz ? QSize( totWidth, textHeight + 7 ) :
						  QSize( textHeight + 7, totWidth );
		if( sh != mSizeHint )
		{
			mSizeHint = sh;
			qDebug() << "SH=" << sh;
			mWidget->updateGeometry();
		}
	}

	void TabBar::Private::paint()
	{
		QFont font1 = mWidget->font();
		QFont font2 = font1;
		font2.setBold( true );

		bool horz = isHorizontal( mTabPos );

		QPainter p( mWidget );
		QRect me = mWidget->contentsRect();

		calculate();

		QColor base( QColor( 64, 64, 64 ) );

		QLinearGradient br( 0, 0, horz ? 0 : me.width(), horz ? me.height() : 0 );
		br.setColorAt( 0, base );
		br.setColorAt( 0.8, base.lighter() );
		br.setColorAt( 1, base.light( 200 ) );
		QBrush back( br );
		p.fillRect( me, back );
		p.setPen( Qt::black );

		if( horz )
		{
			p.drawLine( 0, 0, me.width(), 0 );
			p.drawLine( 0, me.height() - 1, me.width(), me.height() - 1 );
		}
		else
		{
			p.drawLine( 0, 0, 0, me.height() );
			p.drawLine( me.width() - 1, 0, me.width() - 1, me.height() );
		}

		for( int i = 0; i < mInfos.count(); i++ )
		{
			const Private::TabInfo& ti = mInfos[ i ];

			QColor back = ( i == mCurrent ) ? base.lighter( 175 ) : base.lighter( 300 );
			QColor border = ( i == mCurrent ) ? base.lighter( 250 ) : base.darker( 150 );
			QColor text = ( i == mCurrent ) ? base.lighter( 400 ) : base.lighter( 175 );

			p.fillRect( ti.mRect, back );
			p.setPen( border );
			p.drawRect( ti.mRect );
			p.setPen( text );

			p.setFont( i == mCurrent ? font2 : font1 );
			QRect txtrect = ti.mRect.adjusted( 2, 2, -2, -2 );

			if( horz )
			{
				p.drawText( txtrect, Qt::AlignCenter, ti.mName );
			}
			else
			{
				p.save();
				p.translate( txtrect.bottomLeft() );
				p.rotate( 270 );
				QRect r( 0, 0, txtrect.height(), txtrect.width() );
//				p.fillRect( r, Qt::yellow );
				p.drawText( r, Qt::AlignCenter, ti.mName );
				p.restore();
			}
		}

	}

	TabBar::TabBar( QWidget* parent )
		: QWidget( parent )
	{
		d = new Private;
		d->mWidget = this;
	}

	TabBar::~TabBar()
	{
		delete d;
	}

	int TabBar::addTab( const QString& tabName )
	{
		return insertTab( count(), tabName );
	}

	int TabBar::insertTab( int before, const QString& tabName )
	{
		Private::TabInfo ti;
		ti.mName = tabName;
		d->mInfos.insert( before, ti );

		if( d->mCurrent == -1 )
		{
			setCurrentIndex( 0 );
		}
		else
		{
			update();
		}

		return before;
	}

	void TabBar::removeTab( int index )
	{
		Q_ASSERT( index >= 0 && index < count() );
		if( index < 0 || index >= count() )
			return;

		d->mInfos.remove( index );

		if( index == d->mCurrent )
		{
			if( index > 0 )
			{
				setCurrentIndex( index - 1 );
				return;
			}
			if( d->mInfos.count() > 0 )
			{
				d->mCurrent = -1;	// fake a change
				setCurrentIndex( 0 );
				return;
			}
		}

		update();
	}

	void TabBar::setText( int index, const QString tabName )
	{
		Q_ASSERT( index >= 0 && index < count() );
		if( index < 0 || index >= count() )
			return;

		d->mInfos[ index ].mName = tabName;
		update();
	}

	void TabBar::setCurrentIndex( int index )
	{
		if( index != d->mCurrent )
		{
			d->mCurrent = index;
			update();
			emit currentChanged( d->mCurrent );
		}
	}

	int TabBar::currentIndex()
	{
		return d->mCurrent;
	}

	int TabBar::count()
	{
		return d->mInfos.count();
	}

	QSize TabBar::sizeHint() const
	{
		if( !d->mSizeHint.isValid() )
		{
			const_cast< Private* >( d )->calculate();
		}
		return d->mSizeHint;
	}

	QSize TabBar::minimumSizeHint() const
	{
		if( !d->mMinSize.isValid() )
		{
			const_cast< Private* >( d )->calculate();
		}
		return d->mMinSize;
	}

	bool TabBar::event( QEvent* ev )
	{
		return d->event( ev );
	}


	void TabBar::setTabPos( TabBar::TabPos pos )
	{
		d->mTabPos = pos;
		d->calculate();
		update();
	}

	TabBar::TabPos TabBar::tabPos() const
	{
		return d->mTabPos;
	}

}
