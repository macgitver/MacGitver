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
#include <QSplitter>
#include <QVBoxLayout>

#include "GitWrap/Index.h"
#include "GitWrap/DiffList.h"

#include "Diff/Model/GitPatchConsumer.h"
#include "Diff/Model/Patch.h"
#include "Diff/RawView/DiffRawView.h"

#include "IndexWidget.h"
#include "IndexTree.h"

IndexWidget::IndexWidget()
{
	mChangingFilters = false;

	mTree = new IndexTree;
	mTree->setFrameShape( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );

	setupActions( this );

	l->addWidget( tbWorkingTree->toolBarFor( this ) );

	mRawDiff = new DiffRawView;

	mSplitter = new QSplitter( Qt::Horizontal );
	mSplitter->addWidget( mTree );
	mSplitter->addWidget( mRawDiff );

	l->addWidget( mSplitter );
	setLayout( l );

	setViewName( trUtf8( "Working tree" ) );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	mTree->setRepository( repo );

	if( mRepo.isValid() )
	{
		Git::DiffList dl = mRepo.diffIndexToWorkingDir();
		GitPatchConsumer p;
		dl.consumePatch( &p );

		Patch* patch = p.patch();
		QString s = patch->toString();
		delete patch;

		mRawDiff->setText( s );
	}
}

void IndexWidget::onShowAll( bool enabled )
{
	if( !mChangingFilters )
	{
		mChangingFilters = true;

		mTree->setFilter( enabled ? IndexTree::All : IndexTree::None );

		actShowIgnored->setChecked( enabled );
		actShowMissing->setChecked( enabled);
		actShowModified->setChecked( enabled );
		actShowUnchanged->setChecked( enabled );
		actShowUntracked->setChecked( enabled );

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

		actShowAll->setChecked( f == IndexTree::All );

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

		actShowAll->setChecked( f == IndexTree::All );

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

		actShowAll->setChecked( f == IndexTree::All );

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

		actShowAll->setChecked( f == IndexTree::All );

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

		actShowAll->setChecked( f == IndexTree::All );

		mChangingFilters = false;
	}
}

