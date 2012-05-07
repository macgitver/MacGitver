
#include <QPainter>
#include <QScrollBar>
#include <QFontMetrics>

#include "Views/Diff/Diff.h"
#include "Views/Diff/DiffDisplay.h"

DiffDisplayWidget::DiffDisplayWidget()
{
	mDiff = NULL;
	mFont = QFont( "Monospace", 10 );

}

DiffDisplayWidget::~DiffDisplayWidget()
{

}

void DiffDisplayWidget::setDifference( Difference* diff )
{
	mDiff = diff;

	calculateHeights();
}

void DiffDisplayWidget::calculateHeights()
{
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
}

void DiffDisplayWidget::paintEvent( QPaintEvent* ev )
{
	QPainter p( viewport() );
}

void DiffDisplayWidget::resizeEvent( QResizeEvent* ev )
{
	calculateHeights();
	QAbstractScrollArea::resizeEvent( ev );
}
