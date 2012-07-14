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

#ifndef MGV_BRANCHES_VIEW_H
#define MGV_BRANCHES_VIEW_H

#include "Heaven/Views/View.h"

#include "GitWrap/Repository.h"

class QListWidget;
class QToolBar;
class QToolButton;

class BranchesView : public Heaven::View
{
	Q_OBJECT
public:
	BranchesView();

public:
	void repositoryChanged( Git::Repository repo );

public:
	virtual QSize sizeHint() const;

private slots:
	void rereadBranches();

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
	QToolBar*			mToolBar;
	QToolButton*		mBtnLocals;
	QToolButton*		mBtnRemotes;
};

#endif
