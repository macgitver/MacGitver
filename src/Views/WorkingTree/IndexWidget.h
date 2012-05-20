/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include <QWidget>

class QToolBar;

#include "GitWrap/Git.h"
#include "GitWrap/Repository.h"

#include "Heaven/View.h"

class IndexTree;

class IndexWidget : public HeavenView
{
	Q_OBJECT
public:
	IndexWidget();

private slots:
	void repositoryChanged( Git::Repository repo );

	void onShowAll( bool enabled );
	void onShowModified( bool enabled );
	void onShowMissing( bool enabled );
	void onShowIgnored( bool enabled );
	void onShowUntracked( bool enabled );
	void onShowUnchanged( bool enabled );

private:
	IndexTree*			mTree;
	QToolBar*			mToolBar;
	Git::Repository		mRepo;

	bool				mChangingFilters;

	QAction*			mactShowAll;
	QAction*			mactShowUnchanged;
	QAction*			mactShowModified;
	QAction*			mactShowMissing;
	QAction*			mactShowIgnored;
	QAction*			mactShowUntacked;
};

#endif

