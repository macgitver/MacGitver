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

#include "libGitWrap/ObjectId.h"
#include "libGitWrap/ObjectCommit.h"
#include "libGitWrap/Repository.h"

#include "Diff/Model/Patch.h"

class QToolBar;
class QTreeView;
class QComboBox;

class DiffRawView;

	//Libs/Widgets
class SHA1Input;
class ShortCommitModel;
class FlatTreeComboBox;

class HistoryDiff : public QWidget
{
	Q_OBJECT
public:
	HistoryDiff();

public:
	void setRepository( Git::Repository repo );

public:
	void setCommitId( const Git::ObjectId& sha1 );

private slots:
	void onDiffToChanged( int index );
	void createPatch();

private:
	Git::DiffList makePatchTo( const QString& ref );
	void setPatch( Patch::Ptr patchFile );

private:
	enum DiffToTypes
	{
		DTT_WT = -1,
		DTT_Index = -2,
		DTT_WT_and_Index = -3,
		DTT_HEAD = -4,
		DTT_AllParents = -5,
		DTT_AnySHA1 = -6,
		DTT_Parent = -7,
		DTT_Branch = -8,
		DTT_Tag = -9
	};

	DiffRawView*		mRawView;
	QToolBar*			mToolbar;
	QComboBox*			mDiffTo;
	FlatTreeComboBox*	mDiffToBranch;
	FlatTreeComboBox*	mDiffToTag;
	QComboBox*			mDiffToParent;
	SHA1Input*			mDiffToSha1;
	ShortCommitModel*	mParentsModel;
	QTreeView*			mParentsList;

	QHash< DiffToTypes,
		QAction* >		mDiffToActions;

	Git::Repository		mRepo;
	Git::ObjectCommit	mCommit;
};

#endif
