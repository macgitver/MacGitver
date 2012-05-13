
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSplitter>

#include "Libs/Heaven/HDecorator.h"
#include "Libs/Heaven/HContainer.h"
#include "Libs/Heaven/HView.h"

HeavenContainer::HeavenContainer( Type t , Type s, HeavenContainer* parent )
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

HeavenContainer::~HeavenContainer()
{
}

#if 0
void HeavenContainer::replaceWith( HeavenContainer* newContainer )
{
	HeavenContainer* p = qobject_cast< HeavenContainer* >( parent() );
	int i = p->indexOf( this );

}
#endif

HeavenContainer::Type HeavenContainer::type() const
{
	return mType;
}

QList< HeavenView* > HeavenContainer::views() const
{
	QList< HeavenView* > r;

	for( int j = 0; j < mContent.count(); j++ )
		if( mContent[ j ].isView )
			r << mContent[ j ].view;

	return r;
}

int HeavenContainer::numViews() const
{
	int n = 0;

	for( int j = 0; j < mContent.count(); j++ )
		if( mContent[ j ].isView )
			n++;

	return n;
}

QList< HeavenContainer* > HeavenContainer::containers() const
{
	QList< HeavenContainer* > r;

	for( int j = 0; j < mContent.count(); j++ )
		if( !mContent[ j ].isView )
			r << mContent[ j ].container;

	return r;
}

int HeavenContainer::numContainers() const
{
	int n = 0;

	for( int j = 0; j < mContent.count(); j++ )
		if( ! mContent[ j ].isView )
			n++;

	return n;
}

int HeavenContainer::addView( HeavenView* view )
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

int HeavenContainer::addContainer( HeavenContainer* container )
{
	int pos = mContent.count();
	insertContainer( pos, container );
	return pos;
}

void HeavenContainer::insertContainer( int pos, HeavenContainer* container )
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

HeavenView* HeavenContainer::takeView( int index )
{
	return 0;
}

QWidget* HeavenContainer::containerWidget()
{
	return mContainerWidget;
}
