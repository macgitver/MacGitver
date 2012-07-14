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

#ifndef MGV_HISTORY_VIEW_H
#define MGV_HISTORY_VIEW_H

#include <QWidget>
#include <QItemDelegate>

class QToolBar;

#include "GitWrap/Git.h"
#include "GitWrap/Repository.h"

#include "Heaven/Views/View.h"

#include "HistoryEntry.h"

class HistoryBuilder;
class HistoryModel;
class HistoryList;

class HistoryViewDelegate : public QItemDelegate
{
public:
	void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
	void paintGraphLane( QPainter* p, GraphGlyphs glyph, int x1, int x2, const QColor& col,
						 const QColor& activeCol, const QBrush& back ) const;
	void paintGraph( QPainter* p, const QStyleOptionViewItem& opt,
										  const QModelIndex& i) const;
};

class HistoryView : public Heaven::View
{
	Q_OBJECT
public:
	HistoryView();

public:
	void repositoryChanged( Git::Repository repo );

private:
	void buildHistory();

private:
	HistoryModel*		mModel;
	HistoryList*		mList;
	HistoryBuilder*		mBuilder;
	QToolBar*			mToolBar;
	Git::Repository		mRepo;
};

#endif

