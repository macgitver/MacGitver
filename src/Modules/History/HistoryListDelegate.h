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

#ifndef MGV_HISTORY_VIEW_DELEGATE_H
#define MGV_HISTORY_VIEW_DELEGATE_H

#include <QItemDelegate>

#include "HistoryEntry.h"

class HistoryListDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	void paint( QPainter* painter, const QStyleOptionViewItem& option,
				const QModelIndex& index ) const;

	QSize sizeHint( const QStyleOptionViewItem& option,
					const QModelIndex& index) const;

private:
	void paintGraphLane( QPainter* p, GraphGlyphs glyph, int x1, int x2, int height,
						 const QColor& col, const QColor& activeCol, const QBrush& back ) const;

	void paintGraph( QPainter* p, const QStyleOptionViewItem& opt,
					 const QModelIndex& i ) const;

	void paintMessage( QPainter* p, const QStyleOptionViewItem& opt,
					   const QModelIndex& i ) const;
};

#endif
