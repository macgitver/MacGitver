
#include <QPainter>
#include <QPalette>
#include <QVBoxLayout>

#include "Heaven/HTopLevelWidget.h"

HeavenTopLevelWidget::HeavenTopLevelWidget()
{
	mRoot1 = new HeavenContainer( HeavenContainer::Splitter, HeavenContainer::SubSplitVert );
	mRoot2 = new HeavenContainer( HeavenContainer::Splitter, HeavenContainer::SubSplitHorz );
	mRoot1->addContainer( mRoot2 );

	mDocks[ 0 ] = new HeavenContainer( HeavenContainer::Tab, HeavenContainer::SubTabLeft );
	mDocks[ 1 ] = new HeavenContainer( HeavenContainer::Tab, HeavenContainer::SubTabRight );
	mDocks[ 2 ] = new HeavenContainer( HeavenContainer::Tab, HeavenContainer::SubTabTop );
	mDocks[ 3 ] = new HeavenContainer( HeavenContainer::Tab, HeavenContainer::SubTabBottom );
	mDocks[ 4 ] = new HeavenContainer( HeavenContainer::Tab, HeavenContainer::SubTabTop );

	mRoot2->addContainer( mDocks[ 4 ] );

	QVBoxLayout* l = new QVBoxLayout;
	l->setMargin( 2 );
	l->setSpacing( 0 );
	l->addWidget( mRoot1->containerWidget() );
	setLayout( l );

	setAutoFillBackground( false );
}

HeavenTopLevelWidget::~HeavenTopLevelWidget()
{
}

void HeavenTopLevelWidget::addView( HeavenView* view, Heaven::Positions pos )
{
	switch( pos )
	{
	case Heaven::Left:
		if( mRoot2->containers().first() != mDocks[ 0 ] )
		{
			mRoot2->insertContainer( 0, mDocks[ 0 ] );
		}

		mDocks[ 0 ]->addView( view );

		break;

	case Heaven::Right:
		if( mRoot2->containers().last() != mDocks[ 1 ] )
		{
			mRoot2->insertContainer( mRoot2->numContainers(), mDocks[ 1 ] );
		}

		mDocks[ 1 ]->addView( view );

		break;

	case Heaven::Top:
		if( mRoot1->containers().last() != mDocks[ 2 ] )
		{
			mRoot1->insertContainer( mRoot1->numContainers(), mDocks[ 2 ] );
		}

		mDocks[ 2 ]->addView( view );

		break;

	case Heaven::Bottom:
		if( mRoot1->containers().last() != mDocks[ 3 ] )
		{
			mRoot1->insertContainer( mRoot1->numContainers(), mDocks[ 3 ] );
		}

		mDocks[ 3 ]->addView( view );

		break;

	case Heaven::Central:
		mDocks[ 4 ]->addView( view );
		break;

	default:
		break;
	}
}

void HeavenTopLevelWidget::paintEvent( QPaintEvent* ev )
{
	return;
	QPainter p( this );
	p.fillRect( contentsRect(), QColor( "navy" ) );
}
