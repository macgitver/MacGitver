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
#include <QTableWidget>
#include <QSplitter>

#include "Heaven/Views/Decorator.h"
#include "Heaven/Views/Container.h"
#include "Heaven/Views/View.h"

HeavenViewContainer::HeavenViewContainer( Type t , Type s, HeavenViewContainer* parent )
	: QObject( parent )
	, mType( Type( t | s ) )
	, mContainerWidget( NULL )
{
	switch( t & BaseMask )
	{
	case Tab:
		mTabWidget = new QTabWidget;
		switch( s )
		{
		case SubTabLeft:	mTabWidget->setTabPosition( QTabWidget::West );		break;
		case SubTabRight:	mTabWidget->setTabPosition( QTabWidget::East );		break;
		case SubTabTop:		mTabWidget->setTabPosition( QTabWidget::North );	break;
		case SubTabBottom:	mTabWidget->setTabPosition( QTabWidget::South );	break;
		default: break;
		}

		break;

	case Splitter:
		mSpliterWidget = new QSplitter;
		break;

	default:
		Q_ASSERT( false );
		break;
	}
}

HeavenViewContainer::~HeavenViewContainer()
{
}

#if 0
void HeavenContainer::replaceWith( HeavenContainer* newContainer )
{
	HeavenContainer* p = qobject_cast< HeavenContainer* >( parent() );
	int i = p->indexOf( this );

}
#endif

HeavenViewContainer::Type HeavenViewContainer::type() const
{
	return mType;
}

QList< HeavenView* > HeavenViewContainer::views() const
{
	QList< HeavenView* > r;

	for( int j = 0; j < mContent.count(); j++ )
		if( mContent[ j ].isView )
			r << mContent[ j ].view;

	return r;
}

int HeavenViewContainer::numViews() const
{
	int n = 0;

	for( int j = 0; j < mContent.count(); j++ )
		if( mContent[ j ].isView )
			n++;

	return n;
}

QList< HeavenViewContainer* > HeavenViewContainer::containers() const
{
	QList< HeavenViewContainer* > r;

	for( int j = 0; j < mContent.count(); j++ )
		if( !mContent[ j ].isView )
			r << mContent[ j ].container;

	return r;
}

int HeavenViewContainer::numContainers() const
{
	int n = 0;

	for( int j = 0; j < mContent.count(); j++ )
		if( ! mContent[ j ].isView )
			n++;

	return n;
}

int HeavenViewContainer::addView( HeavenView* view )
{
	Content ct;
	ct.isView = true;
	ct.view = view;
	mContent.append( ct );

	switch( mType & BaseMask )
	{
	case Tab:
		return mTabWidget->addTab( view, view->viewName() );

	case Splitter:
		{
			QWidget* wrapper = new QWidget;
			QVBoxLayout* l = new QVBoxLayout;
			l->setSpacing( 0 );
			l->setMargin( 0 );
			l->addWidget( new HeavenDecorator( view ) );
			l->addWidget( view );
			wrapper->setLayout( l );
			mSpliterWidget->addWidget( wrapper );
		}
		return mContent.count() - 1;

	default:
		Q_ASSERT( false );
		return -1;
	}
}

int HeavenViewContainer::addContainer( HeavenViewContainer* container )
{
	int pos = mContent.count();
	insertContainer( pos, container );
	return pos;
}

void HeavenViewContainer::insertContainer( int pos, HeavenViewContainer* container )
{
	Content ct;
	ct.isView = false;
	ct.container = container;
	mContent.insert( pos, ct );

	switch( mType & BaseMask )
	{
	case Tab:
		mTabWidget->insertTab( pos, container->containerWidget(), "Container" );
		return;

	case Splitter:
		mSpliterWidget->insertWidget( pos, container->containerWidget() );
		return;

	default:
		Q_ASSERT( false );
		return;
	}
}

HeavenView* HeavenViewContainer::takeView( int index )
{
	return 0;
}

QWidget* HeavenViewContainer::containerWidget()
{
	return mContainerWidget;
}
