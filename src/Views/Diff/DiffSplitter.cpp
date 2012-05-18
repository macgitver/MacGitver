
#include <QPainter>

#include "DiffSplitter.h"

DiffSplitter::DiffSplitter( QWidget* parent )
	: QSplitter( Qt::Horizontal, parent )
{
	setHandleWidth( 50 );
}

QSplitterHandle* DiffSplitter::createHandle()
{
	return new DiffSplitterHandle( orientation(), this );
}

//--------------------------

DiffSplitterHandle::DiffSplitterHandle( Qt::Orientation o, DiffSplitter *parent )
	: QSplitterHandle( o, parent )
{
}

void DiffSplitterHandle::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );

	QPainter p( this );

	p.fillRect( contentsRect(), palette().color( QPalette::Background ) );
}
