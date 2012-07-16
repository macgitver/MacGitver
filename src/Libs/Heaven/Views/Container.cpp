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

namespace Heaven
{

	ViewContainer::ViewContainer( Type t, Type s, ViewContainer* parent )
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
			mSpliterWidget = new QSplitter( s == SubSplitHorz ? Qt::Horizontal : Qt::Vertical );
			break;

		default:
			Q_ASSERT( false );
			break;
		}
	}

	ViewContainer::~ViewContainer()
	{
	}

	ViewContainer::Type ViewContainer::type() const
	{
		return mType;
	}

	QList< View* > ViewContainer::views() const
	{
		QList< View* > r;

		for( int j = 0; j < mContents.count(); j++ )
		{
			if( !mContents[ j ]->isContainer() )
			{
				r.append( mContents[ j ]->asView() );
			}
		}

		return r;
	}

	int ViewContainer::numViews() const
	{
		int n = 0;

		for( int j = 0; j < mContents.count(); j++ )
		{
			if( !mContents[ j ]->isContainer() )
			{
				n++;
			}
		}

		return n;
	}

	QList< ViewContainer* > ViewContainer::containers() const
	{
		QList< ViewContainer* > r;

		for( int j = 0; j < mContents.count(); j++ )
		{
			if( mContents[ j ]->isContainer() )
			{
				r.append( mContents[ j ]->asContainer() );
			}
		}

		return r;
	}

	int ViewContainer::numContainers() const
	{
		int n = 0;

		for( int j = 0; j < mContents.count(); j++ )
		{
			if( mContents[ j ]->isContainer() )
			{
				n++;
			}
		}

		return n;
	}

	int ViewContainer::addView( View* view )
	{
		mContents.append( view );

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
				l->addWidget( new Decorator( view ) );
				l->addWidget( view );
				wrapper->setLayout( l );
				mSpliterWidget->addWidget( wrapper );
			}
			return mContents.count() - 1;

		default:
			Q_ASSERT( false );
			return -1;
		}
	}

	int ViewContainer::addContainer( ViewContainer* container )
	{
		int pos = mContents.count();
		insertContainer( pos, container );
		return pos;
	}

	void ViewContainer::insertContainer( int pos, ViewContainer* container )
	{
		mContents.insert( pos, container );

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

	View* ViewContainer::takeView( int index )
	{
		return 0;
	}

	QWidget* ViewContainer::containerWidget()
	{
		return mContainerWidget;
	}

	bool ViewContainer::isContainer() const
	{
		return true;
	}

	ViewContainer* ViewContainer::asContainer()
	{
		return this;
	}

	QList< ContainerContent* > ViewContainer::contents() const
	{
		return mContents;
	}

}
