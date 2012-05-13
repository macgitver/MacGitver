
#include "Libs/Heaven/HView.h"

HeavenView::HeavenView()
{
	mAction = NULL;
}

QString HeavenView::viewName() const
{
	return mViewName;
}

void HeavenView::setViewName( const QString& name )
{
	if( name != mViewName )
	{
		mViewName = name;
		emit nameChanged( mViewName );
	}
}
