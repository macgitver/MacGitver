
#include <QPainter>
#include <QScrollBar>
#include <QFontMetrics>

#include "Libs/Diff/Diff.h"
#include "Views/Diff/DiffWidget.h"

DiffWidget::DiffWidget()
{
	mDiff = NULL;
	mFont = QFont( "Monospace", 10 );
}

DiffWidget::~DiffWidget()
{

}

void DiffWidget::setDifference( PatchFile* diff )
{
	mDiff = diff;

	calculateHeights();
}

void DiffWidget::calculateHeights()
{
#if 0
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
#endif
}

void DiffWidget::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );
	QPainter p( this );

	QRect r = contentsRect();
	p.fillRect( r, palette().color( QPalette::Base ) );
}

void DiffWidget::resizeEvent( QResizeEvent* ev )
{
	calculateHeights();
	QWidget::resizeEvent( ev );
}
