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

class QToolBar;

#include "GitWrap/Git.h"
#include "GitWrap/Repository.h"

#include "Heaven/Views/GlobalView.h"

class HistoryBuilder;
class HistoryModel;
class HistoryDetails;
class HistoryList;

class HistoryView : public Heaven::GlobalView
{
	Q_OBJECT
public:
	HistoryView();

public slots:
	void repositoryChanged( Git::Repository repo );

private:
	void buildHistory();

private slots:
	void currentCommitChanged( const Git::ObjectId& sh1 );

private:
	HistoryModel*		mModel;
	HistoryList*		mList;
	HistoryDetails*		mDetails;
	HistoryBuilder*		mBuilder;
	QToolBar*			mToolBar;
	Git::Repository		mRepo;
};

#endif

