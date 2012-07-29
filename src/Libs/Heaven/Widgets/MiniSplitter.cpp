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

#include <QPaintEvent>
#include <QPainter>

#include "Heaven/Widgets/MiniSplitter.h"

namespace Heaven
{

	class MiniSplitterHandle : public QSplitterHandle
	{
	public:
		MiniSplitterHandle( Qt::Orientation ori, QSplitter* parent )
			: QSplitterHandle( ori, parent )
		{
			setMask( QRegion( contentsRect() ) );
			setAttribute( Qt::WA_MouseNoMask, true );
		}

	protected:
		void resizeEvent( QResizeEvent* ev )
		{
			if (orientation() == Qt::Horizontal)
				setContentsMargins(2, 0, 2, 0);
			else
				setContentsMargins(0, 2, 0, 2);

			setMask( QRegion( contentsRect() ) );

			QSplitterHandle::resizeEvent( ev );
		}

		void paintEvent( QPaintEvent* ev )
		{
			QPainter p( this );
			p.fillRect( ev->rect(), Qt::black );
		}
	};

	MiniSplitter::MiniSplitter( QWidget* parent )
		: QSplitter( parent )
	{
		init();
	}

	MiniSplitter::MiniSplitter( Qt::Orientation ori )
		: QSplitter( ori )
	{
		init();
	}

	void MiniSplitter::init()
	{
		setHandleWidth( 1 );
		setChildrenCollapsible( false );
	}

	QSplitterHandle* MiniSplitter::createHandle()
	{
		return new MiniSplitterHandle( orientation(), this );
	}

}
