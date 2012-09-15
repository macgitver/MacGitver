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

#ifndef MGV_HISTORY_DIFF_H
#define MGV_HISTORY_DIFF_H

#include <QWidget>

#include "GitWrap/ObjectId.h"
#include "GitWrap/ObjectCommit.h"
#include "GitWrap/Repository.h"

class QToolBar;
class QComboBox;

class DiffRawView;
class SHA1Input;

class HistoryDiff : public QWidget
{
	Q_OBJECT
public:
	HistoryDiff();

public:
	void setRepository( Git::Repository repo );

public slots:
	void setCommitId( const Git::ObjectId& sha1 );

private:
	DiffRawView*		mRawView;
	QToolBar*			mToolbar;
	QComboBox*			mDiffTo;
	SHA1Input*			mSha1Input;
	Git::Repository		mRepo;
	Git::ObjectCommit	mCommit;

	enum DiffToTypes
	{
		DTT_WT = -1,
		DTT_Index = -2,
		DTT_WT_and_Index = -3,
		DTT_HEAD = -4,
		DTT_AllParents = -5,
		DTT_AnySHA1 = -6,
		DTT_Branch = -7,
		DTT_Tag = -8
	};
};

#endif
