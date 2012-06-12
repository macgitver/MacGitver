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

#ifndef DIFF_VIEW_H
#define DIFF_VIEW_H

#include <QWidget>

#include "Heaven/View.h"

#include "Diff/View/DiffViewApi.h"

class QTreeWidget;
class QStackedWidget;
class QTextBrowser;
class QScrollBar;

class Patch;
class DiffWidget;
class DiffTreeFileList;
class DiffFrame;

class DIFF_VIEW_API DiffView : public HeavenView
{
	Q_OBJECT
public:
	DiffView();
	~DiffView();

public:
	void setPatch( Patch* patch );

private:
	void clearTree();
	void fillTree();

private:
	Patch*				mPatch;
	DiffTreeFileList*	mTree;
	QTreeWidget*		mDetails;
	QStackedWidget*		mDiffStack;
	QTextBrowser*		mRawDiffView;
	DiffFrame*			mDiffFrame;
	QScrollBar*			mScrollV;
	QScrollBar*			mScrollH;
};

#endif
