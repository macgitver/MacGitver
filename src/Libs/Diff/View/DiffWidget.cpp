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

#include <QPainter>
#include <QScrollBar>
#include <QFontMetrics>

#include "Diff/Model/Diff.h"
#include "DiffWidget.h"

DiffWidget::DiffWidget()
{
	mDiff = NULL;
	mFont = QFont( QLatin1String( "Monospace" ), 10 );
}

DiffWidget::~DiffWidget()
{

}

void DiffWidget::setDifference( PatchFile* diff )
{
	mDiff = diff;

	calculateHeights();
}

void DiffWidget::calculateHeights()
{
#if 0
	QFontMetrics fm( mFont );
	int xHeight = fm.xHeight();

	if( mDiff )
	{
		int lines = mDiff->maximumDisplayLines();
		mTotalHeight = xHeight * lines;
	}
	else
	{
		mTotalHeight = 0;
	}

	QScrollBar* vb = verticalScrollBar();
	QRect cr = viewport()->contentsRect();

	if( mTotalHeight > cr.height() )
	{
		vb->setRange( 0, mTotalHeight - cr.height() );
	}
	else
	{
		vb->setRange( 0, 0 );
	}
#endif
}

void DiffWidget::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );
	QPainter p( this );

	QRect r = contentsRect();
	p.fillRect( r, palette().color( QPalette::Base ) );
}

void DiffWidget::resizeEvent( QResizeEvent* ev )
{
	calculateHeights();
	QWidget::resizeEvent( ev );
}
