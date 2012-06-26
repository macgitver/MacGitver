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

#include "Heaven/Views/Decorator.h"
#include "Heaven/Views/View.h"

HeavenDecorator::HeavenDecorator( HeavenView* view )
	: mView( view )
{
	setMinimumHeight( 20 );
}

void HeavenDecorator::paintEvent( QPaintEvent* ev )
{
	QPainter p( this );

	QRect r = contentsRect().adjusted( 0, 0, -1, -1 );
	p.fillRect( r, Qt::white );
	p.setPen( Qt::black );
	p.drawRect( r );
	r.adjust( 2, 2, -2, -2 );
	p.setFont( font() );
	p.drawText( r, mView->viewName() );
}
