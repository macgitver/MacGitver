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

#include "libGitWrap/Index.h"
#include "libGitWrap/DiffList.h"

#include "libDiffViews/Model/FilePatch.hpp"
#include "libDiffViews/RawView/RawView.hpp"

#include "App/MacGitver.hpp"
#include "MacGitver/FSWatcher.h"
#include "MacGitver/GitPatchConsumer.hpp"

#include "IndexWidget.h"

#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"

IndexWidget::IndexWidget()
    : View( QLatin1String( "WorkTree" ) )
{
    mTreeView = new WorkingTreeItemView;
    mTreeView->setFrameShape( QFrame::NoFrame );

    QVBoxLayout* l = new QVBoxLayout;
    l->setSpacing( 0 );
    l->setMargin( 0 );

    setupActions( this );

//  l->addWidget( tbWorkingTree->toolBarFor( this ) );

    mDiffView = new DiffViews::RawView;

    mSplitter = new Heaven::MiniSplitter( Qt::Horizontal );
    mSplitter->addWidget( mTreeView );
    mSplitter->addWidget( mDiffView );

    l->addWidget( mSplitter );
    setLayout( l );

    setViewName( trUtf8( "Working tree" ) );

    mFilterRecursion = true;
    WorkingTreeFilters filters = mTreeView->filters();

    actShowAll->setChecked( filters == WTF_All );
    actShowIgnored->setChecked( filters & WTF_Ignored );
    actShowMissing->setChecked( filters & WTF_Missing );
    actShowModified->setChecked( filters & WTF_Changed );
    actShowUnchanged->setChecked( filters & WTF_Unchanged );
    actShowUntracked->setChecked( filters & WTF_Untracked );

    mFilterRecursion = false;

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    connect( MacGitver::self().watcher(), SIGNAL(workingTreeChanged()),
             this, SLOT(workingTreeChanged()) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
    mRepo = repo;
    mTreeView->setRepository( repo );

    if( mRepo.isValid() )
    {
        updateDiff();
    }
}

void IndexWidget::updateDiff()
{
    Git::Result r;
    Git::DiffList dl = mRepo.diffIndexToWorkingDir( r );
    GitPatchConsumer p;
    dl.consumePatch( &p, r );

    mDiffView->setPatch( p.patch() );
}

void IndexWidget::onShowAll( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    setTreeFilter( enabled ? WTF_All : WTF_None );

    actShowIgnored->setChecked( enabled );
    actShowMissing->setChecked( enabled);
    actShowModified->setChecked( enabled );
    actShowUnchanged->setChecked( enabled );
    actShowUntracked->setChecked( enabled );

    mFilterRecursion = false;
}

void IndexWidget::onShowModified( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilters f = mTreeView->filters();
    if( enabled )
        f |= WTF_Changed;
    else
        f &= ~WTF_Changed;
    setTreeFilter( f );

    actShowAll->setChecked( f == WTF_All );

    mFilterRecursion = false;
}

void IndexWidget::onShowMissing( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilters f = mTreeView->filters();
    if( enabled )
        f |= WTF_Missing;
    else
        f &= ~WTF_Missing;
    setTreeFilter( f );

    actShowAll->setChecked( f == WTF_All );

    mFilterRecursion = false;
}

void IndexWidget::onShowIgnored( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilters f = mTreeView->filters();
    if( enabled )
        f |= WTF_Ignored;
    else
        f &= ~WTF_Ignored;
    setTreeFilter( f );

    actShowAll->setChecked( f == WTF_All );

    mFilterRecursion = false;
}

void IndexWidget::onShowUntracked( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilters f = mTreeView->filters();
    if( enabled )
        f |= WTF_Untracked;
    else
        f &= ~WTF_Untracked;
    setTreeFilter( f );

    actShowAll->setChecked( f == WTF_All );

    mFilterRecursion = false;
}

void IndexWidget::onShowUnchanged( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilters f = mTreeView->filters();
    if( enabled )
        f |= WTF_Unchanged;
    else
        f &= ~WTF_Unchanged;
    setTreeFilter( f );

    actShowAll->setChecked( f == WTF_All );

    mFilterRecursion = false;
}

void IndexWidget::setTreeFilter( WorkingTreeFilters filters )
{
    mTreeView->setFilter( filters );
}

void IndexWidget::workingTreeChanged()
{
    WorkingTreeModel* model = static_cast< WorkingTreeModel* >( mTreeView->model() );
    if( !model )
    {
        return;
    }

    model->update();
    updateDiff();
}
