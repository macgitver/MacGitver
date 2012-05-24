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

#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "GitWrap/Index.h"

#include "Views/WorkingTree/IndexWidget.h"
#include "Views/WorkingTree/IndexTree.h"

IndexWidget::IndexWidget()
{
	mChangingFilters = false;

	mTree = new IndexTree;
	mTree->setFrameShape( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );

	mToolBar = new QToolBar;

	mactShowAll = mToolBar->addAction( trUtf8( "All" ), this, SLOT(onShowAll(bool)) );
	mactShowAll->setCheckable( true );
	mactShowAll->setChecked( true );

	mactShowUnchanged = mToolBar->addAction( trUtf8( "Unchanged" ), this, SLOT(onShowUnchanged(bool)) );
	mactShowUnchanged->setCheckable( true );
	mactShowUnchanged->setChecked( true );

	mactShowModified = mToolBar->addAction( trUtf8( "Modified" ), this, SLOT(onShowModified(bool)) );
	mactShowModified->setCheckable( true );
	mactShowModified->setChecked( true );

	mactShowMissing = mToolBar->addAction( trUtf8( "Missing" ), this, SLOT(onShowMissing(bool)) );
	mactShowMissing->setCheckable( true );
	mactShowMissing->setChecked( true );

	mactShowIgnored = mToolBar->addAction( trUtf8( "Ignored" ), this, SLOT(onShowIgnored(bool)) );
	mactShowIgnored->setCheckable( true );
	mactShowIgnored->setChecked( true );

	mactShowUntacked = mToolBar->addAction( trUtf8( "Untracked" ), this, SLOT(onShowUntracked(bool)) );
	mactShowUntacked->setCheckable( true );
	mactShowUntacked->setChecked( true );

	mToolBar->addSeparator();

	mToolBar->addAction( "Stage" );
	mToolBar->addAction( "Commit" );
	mToolBar->addAction( "Reset" );

	l->addWidget( mToolBar );

	l->addWidget( mTree );
	setLayout( l );

	setViewName( trUtf8( "Working tree" ) );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	mTree->setRepository( repo );
}

void IndexWidget::onShowAll( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		mTree->setFilter( enabled ? IndexTree::All : IndexTree::None );

		mactShowIgnored->setChecked( enabled );
		mactShowMissing->setChecked( enabled);
		mactShowModified->setChecked( enabled );
		mactShowUnchanged->setChecked( enabled );
		mactShowUntacked->setChecked( enabled );

		mChangingFilters = false;
	}
}

void IndexWidget::onShowModified( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		IndexTree::TreeFilters f = mTree->filters();
		if( enabled )
			f |= IndexTree::Changed;
		else
			f &= ~IndexTree::Changed;
		mTree->setFilter( f );

		mactShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}
}

void IndexWidget::onShowMissing( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		IndexTree::TreeFilters f = mTree->filters();
		if( enabled )
			f |= IndexTree::Missing;
		else
			f &= ~IndexTree::Missing;
		mTree->setFilter( f );

		mactShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}

}

void IndexWidget::onShowIgnored( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		IndexTree::TreeFilters f = mTree->filters();
		if( enabled )
			f |= IndexTree::Ignored;
		else
			f &= ~IndexTree::Ignored;
		mTree->setFilter( f );

		mactShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}
}

void IndexWidget::onShowUntracked( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		IndexTree::TreeFilters f = mTree->filters();
		if( enabled )
			f |= IndexTree::Untracked;
		else
			f &= ~IndexTree::Untracked;
		mTree->setFilter( f );

		mactShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}
}

void IndexWidget::onShowUnchanged( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		IndexTree::TreeFilters f = mTree->filters();
		if( enabled )
			f |= IndexTree::Unchanged;
		else
			f &= ~IndexTree::Unchanged;
		mTree->setFilter( f );

		mactShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}
}

