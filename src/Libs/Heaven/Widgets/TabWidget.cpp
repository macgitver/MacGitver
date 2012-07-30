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

#include <QVBoxLayout>
#include <QStackedWidget>

#include "TabBar.h"
#include "TabWidget.h"

namespace Heaven
{

	TabWidget::TabWidget( QWidget* parent )
		: QWidget( parent )
	{
		mBar = new TabBar;
		mStack = new QStackedWidget;

		QVBoxLayout* l = new QVBoxLayout;
		l->setSpacing( 0 );
		l->setMargin( 0 );

		l->addWidget( mBar );
		l->addWidget( mStack );
		setLayout( l );

		connect( mBar, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)) );
	}

	int TabWidget::addTab( QWidget* widget, const QString& tabName )
	{
		mStack->addWidget( widget );
		mBar->addTab( tabName );
		return count() - 1;
	}

	int TabWidget::insertTab( int before, QWidget* widget, const QString& tabName )
	{
		mStack->insertWidget( before, widget );
		mBar->insertTab( before, tabName );
		return before;
	}

	void TabWidget::removeTab( int index )
	{
		QWidget* w = takeAt( index );
		if( w )
			w->deleteLater();
	}

	void TabWidget::removeTab( QWidget* widget )
	{
		removeTab( indexOf( widget ) );
	}

	QWidget* TabWidget::takeAt( int index )
	{
		if( index < 0 || index >= count() )
			return NULL;

		QWidget* w = mStack->widget( index );
		if( !w )
			return NULL;

		w->hide();
		w->setParent( NULL );

		mBar->removeTab( index );
		//mStack->removeWidget( w );

		return w;
	}

	int TabWidget::indexOf( QWidget* widget )
	{
		return mStack->indexOf( widget );
	}

	int TabWidget::count()
	{
		return mBar->count();
	}

	void TabWidget::tabChanged( int newCurrent )
	{
		mStack->setCurrentIndex( newCurrent );
		emit currentChanged( newCurrent );
	}

	void TabWidget::setTabPos( TabBar::TabPos pos )
	{
		if( pos == mBar->tabPos() )
			return;

		mBar->setTabPos( pos );

		delete layout();

		QLayout* newLayout;

		switch( pos )
		{
		case TabBar::North:
			newLayout = new QVBoxLayout;
			newLayout->addWidget( mBar );
			newLayout->addWidget( mStack );
			break;
		case TabBar::South:
			newLayout = new QVBoxLayout;
			newLayout->addWidget( mStack );
			newLayout->addWidget( mBar );
			break;
		case TabBar::West:
			newLayout = new QHBoxLayout;
			newLayout->addWidget( mBar );
			newLayout->addWidget( mStack );
			break;
		case TabBar::East:
			newLayout = new QHBoxLayout;
			newLayout->addWidget( mStack );
			newLayout->addWidget( mBar );
			break;

		default:
			Q_ASSERT( false );
			return;
		}

		newLayout->setSpacing( 0 );
		newLayout->setMargin( 0 );

		setLayout( newLayout );
	}

}
