
#include <QPainter>

#include "Heaven/HDecorator.h"
#include "Heaven/HView.h"

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
