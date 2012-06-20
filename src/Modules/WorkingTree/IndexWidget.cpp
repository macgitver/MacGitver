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

#include "WorkingTreeItemView.h"

IndexWidget::IndexWidget()
{
	mTree = new IndexTree;
	mTree->setFrameShape( QFrame::NoFrame );

	mTreeView = new WorkingTreeItemView;
	mTreeView->setFrameShape( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );

	setupActions( this );

	l->addWidget( tbWorkingTree->toolBarFor( this ) );

	mRawDiff = new DiffRawView;

	mSplitter = new QSplitter( Qt::Horizontal );

	QSplitter* temp = new QSplitter( Qt::Vertical );
	temp->addWidget( mTree );
	temp->addWidget( mTreeView );

	mSplitter->addWidget( temp );
	mSplitter->addWidget( mRawDiff );

	l->addWidget( mSplitter );
	setLayout( l );

	setViewName( trUtf8( "Working tree" ) );

	setTreeFilter( WTF_All );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	mTree->setRepository( repo );
	mTreeView->setRepository( repo );

	if( mRepo.isValid() )
	{
		Git::DiffList dl = mRepo.diffIndexToWorkingDir();
		GitPatchConsumer p;
		dl.consumePatch( &p );

		mRawDiff->setText( p.patch()->toString() );
	}
}

void IndexWidget::onShowAll( bool enabled )
{
	setTreeFilter( enabled ? WTF_All : WTF_None );

	actShowIgnored->setChecked( enabled );
	actShowMissing->setChecked( enabled);
	actShowModified->setChecked( enabled );
	actShowUnchanged->setChecked( enabled );
	actShowUntracked->setChecked( enabled );
}

void IndexWidget::onShowModified( bool enabled )
{
	WorkingTreeFilters f = mTree->filters();
	if( enabled )
		f |= WTF_Changed;
	else
		f &= ~WTF_Changed;
	setTreeFilter( f );

	actShowAll->setChecked( f == WTF_All );
}

void IndexWidget::onShowMissing( bool enabled )
{
	WorkingTreeFilters f = mTree->filters();
	if( enabled )
		f |= WTF_Missing;
	else
		f &= ~WTF_Missing;
	setTreeFilter( f );

	actShowAll->setChecked( f == WTF_All );
}

void IndexWidget::onShowIgnored( bool enabled )
{
	WorkingTreeFilters f = mTree->filters();
	if( enabled )
		f |= WTF_Ignored;
	else
		f &= ~WTF_Ignored;
	setTreeFilter( f );

	actShowAll->setChecked( f == WTF_All );
}

void IndexWidget::onShowUntracked( bool enabled )
{
	WorkingTreeFilters f = mTree->filters();
	if( enabled )
		f |= WTF_Untracked;
	else
		f &= ~WTF_Untracked;
	setTreeFilter( f );

	actShowAll->setChecked( f == WTF_All );
}

void IndexWidget::onShowUnchanged( bool enabled )
{
	WorkingTreeFilters f = mTree->filters();
	if( enabled )
		f |= WTF_Unchanged;
	else
		f &= ~WTF_Unchanged;
	setTreeFilter( f );

	actShowAll->setChecked( f == WTF_All );
}

void IndexWidget::setTreeFilter( WorkingTreeFilters filters )
{
	mTree->setFilter( filters );
	mTreeView->setFilter( filters );
}

void IndexWidget::workingTreeChanged()
{
}
