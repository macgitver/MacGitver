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

#include <QPainter>
#include <QVariant>

#include "DiffSplitter.h"

class DiffSplitterHandle : public QSplitterHandle
{
public:
	DiffSplitterHandle( DiffSplitter *parent );

protected:
	void paintEvent( QPaintEvent* ev );
};

DiffSplitterHandle::DiffSplitterHandle( DiffSplitter *parent )
	: QSplitterHandle( Qt::Horizontal, parent )
{
}

void DiffSplitterHandle::paintEvent( QPaintEvent* ev )
{
	Q_UNUSED( ev );

	QPainter p( this );

	p.fillRect( contentsRect(), palette().color( QPalette::Background ) );
}


DiffSplitter::DiffSplitter( QWidget* parent )
	: QSplitter( Qt::Horizontal, parent )
{
	setHandleWidth( 50 );
	setProperty( "heavenStyle", false );
}

QSplitterHandle* DiffSplitter::createHandle()
{
	return new DiffSplitterHandle( this );
}

