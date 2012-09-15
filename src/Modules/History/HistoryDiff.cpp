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

#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "Diff/RawView/DiffRawView.h"

#include "Widgets/SHA1Input.h"

#include "HistoryDiff.h"

HistoryDiff::HistoryDiff()
{
	mRawView = new DiffRawView();

	QVBoxLayout* l = new QVBoxLayout;
	l->setMargin( 0 );
	l->setSpacing( 0 );
	setLayout( l );

	mToolbar = new QToolBar;

	mDiffTo = new QComboBox();
	mDiffTo->addItem( trUtf8( "Workingtree" ), DTT_WT );
	mDiffTo->addItem( trUtf8( "Index" ), DTT_Index );
	mDiffTo->addItem( trUtf8( "Workingtree + Index" ), DTT_WT_and_Index );
	mDiffTo->addItem( trUtf8( "HEAD" ), DTT_HEAD );
	mDiffTo->addItem( trUtf8( "SHA-1" ), DTT_AnySHA1 );
	mDiffTo->addItem( trUtf8( "All parents" ), DTT_AllParents );

	mToolbar->addWidget( new QLabel( trUtf8( "Diff to:" ) ) );
	mToolbar->addWidget( mDiffTo );

	l->addWidget( mToolbar );
	l->addWidget( mRawView );
	setLayout( l );
}

void HistoryDiff::setRepository( Git::Repository repo )
{
	mRepo = repo;
}

void HistoryDiff::setCommitId( const Git::ObjectId& sha1 )
{
	mRawView->setText( QString() );

	if( !mRepo.isValid() )
	{
		return;
	}

	mCommit = mRepo.lookupCommit( sha1 );
	if( !mCommit.isValid() )
	{
		return;
	}

	// mRepo.index().read();

}

