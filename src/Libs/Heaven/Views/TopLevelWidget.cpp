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
#include <QPalette>
#include <QVBoxLayout>

#include "Heaven/Views/TopLevelWidget.h"

namespace Heaven
{

	TopLevelWidget::TopLevelWidget()
	{
		mRoot = new ViewContainer( ViewContainer::Splitter, ViewContainer::SubSplitHorz );

		QVBoxLayout* l = new QVBoxLayout;
		l->setMargin( 2 );
		l->setSpacing( 0 );
		l->addWidget( mRoot->containerWidget() );
		setLayout( l );

		setAutoFillBackground( false );
	}

	TopLevelWidget::~TopLevelWidget()
	{
		clear();
	}

	void TopLevelWidget::clear()
	{
	}

	void TopLevelWidget::addView( View* view, Positions pos )
	{
		mRoot->addView( view );
	}

	void TopLevelWidget::addContainer( ViewContainer* c )
	{
		mRoot->addContainer( c );
	}

	ViewContainer* TopLevelWidget::rootContainer()
	{
		return mRoot;
	}

	void TopLevelWidget::setRootContainer( ViewContainer* c )
	{
		clear();
		mRoot = c;
		layout()->addWidget( c->containerWidget() );
	}

	void TopLevelWidget::paintEvent( QPaintEvent* ev )
	{
		return;
		QPainter p( this );
		p.fillRect( contentsRect(), QColor( "navy" ) );
	}

}
