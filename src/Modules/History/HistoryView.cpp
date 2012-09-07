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
#include <QDebug>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>

#include "GitWrap/Reference.h"
#include "GitWrap/ObjectCommit.h"

#include "Diff/RawView/DiffRawView.h"

#include "Heaven/Widgets/MiniSplitter.h"

#include "MacGitver/MacGitver.h"

#include "HistoryView.h"
#include "HistoryEntry.h"
#include "HistoryModel.h"
#include "HistoryBuilder.h"
#include "HistoryList.h"
#include "HistoryListDelegate.h"
#include "HistoryDetails.h"

HistoryView::HistoryView()
	: GlobalView( QLatin1String( "History" ) )
{
	setViewName( trUtf8( "History" ) );
	setSizePolicy( QSizePolicy::MinimumExpanding,
				   QSizePolicy::MinimumExpanding );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );
	setLayout( l );

	mToolBar = new QToolBar;

	QAction* a = mToolBar->addAction( trUtf8( "All Branches" ), this, SLOT(onViewAllBranches()) );
	a->setCheckable( true );

	l->addWidget( mToolBar );

	Heaven::MiniSplitter* ms1 = new Heaven::MiniSplitter( Qt::Vertical );

	mList = new HistoryList;
	mList->setFrameShape( QFrame::NoFrame );
	ms1->addWidget( mList );

	mModel = new HistoryModel( this );
	mList->setModel( mModel );

	connect( mList, SIGNAL(currentCommitChanged(Git::ObjectId)),
			 this, SLOT(currentCommitChanged(Git::ObjectId)) );

	mList->setItemDelegate( new HistoryListDelegate );

	mDetails = new HistoryDetails;
	ms1->addWidget( mDetails );

	Heaven::MiniSplitter* ms2 = new Heaven::MiniSplitter( Qt::Horizontal );
	ms2->addWidget( ms1 );

	DiffRawView* rawDiff = new DiffRawView;
	ms2->addWidget( rawDiff );

	l->addWidget( ms2 );

	mBuilder = NULL;

	connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
			 this, SLOT(repositoryChanged(Git::Repository)) );

	Git::Repository repo = MacGitver::self().repository();
	if( repo.isValid() )
	{
		repositoryChanged( repo );
	}
}

void HistoryView::repositoryChanged( Git::Repository repo )
{
	if( mBuilder )
	{
		delete mBuilder;
		mBuilder = NULL;
	}

	mRepo = repo;
	mModel->setRepository( repo );
	mDetails->setRepository( repo );

	if( mRepo.isValid() )
	{
		buildHistory();
	}
}

void HistoryView::buildHistory()
{
	if( !mRepo.isValid() )
	{
		return;
	}

	if( !mBuilder )
	{
		mBuilder = new HistoryBuilder( mRepo, mModel );
	}

	mBuilder->addHEAD();
	mBuilder->start();
}

void HistoryView::currentCommitChanged( const Git::ObjectId& sha1 )
{
	mDetails->setCommit( sha1 );
}
