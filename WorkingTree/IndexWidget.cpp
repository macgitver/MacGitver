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
#include <QPlainTextEdit>

#include "libGitWrap/Index.hpp"
#include "libGitWrap/DiffList.hpp"

#include "libDiffViews/Model/FilePatch.hpp"
#include "libDiffViews/Views/Interfaces/DiffView.hpp"
#include "libDiffViews/Views/Interfaces/DiffViews.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/MacGitver/FSWatcher.h"
#include "libMacGitverCore/MacGitver/GitPatchConsumer.hpp"

#include "IndexWidget.h"

#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"
#include "WorkingTreeFilterModel.h"

IndexWidget::IndexWidget()
    : View( "WorkTree" )
    , mSplitter( new Heaven::MiniSplitter( Qt::Vertical ) )
    , mTreeView( new WorkingTreeItemView )
    , mIndexTreeView( new WorkingTreeItemView )
    , mCommitMessage( new QPlainTextEdit )
    , mStatusModel( new WorkingTreeModel( this ) )
{
    mTreeView->setFrameShape( QFrame::NoFrame );
    mIndexTreeView->setFrameShape( QFrame::NoFrame );
    mCommitMessage->setFrameShape( QFrame::NoFrame );

    setupActions( this );

    setToolBar( tbWorkingTree );
    setViewName( trUtf8( "Working tree" ) );

    mDiffView = DiffViews::DiffViews::self().defaultCreator()->create( this );

    Heaven::MiniSplitter *hSplit_1 = new Heaven::MiniSplitter( Qt::Horizontal );
    hSplit_1->addWidget( mTreeView );
    hSplit_1->addWidget( mDiffView );

    mSplitter->addWidget( hSplit_1 );

    Heaven::MiniSplitter *hSplit_2 = new Heaven::MiniSplitter( Qt::Horizontal );

    hSplit_2->addWidget( mIndexTreeView );
    hSplit_2->addWidget( mCommitMessage );

    mSplitter->addWidget( hSplit_2 );

    setWidget( mSplitter );

    setupFilters();

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

//  connect( MacGitver::self().watcher(), SIGNAL(workingTreeChanged()),
//           this, SLOT(workingTreeChanged()) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void IndexWidget::setupFilters()
{
    WorkingTreeFilterModel *wtFilter = new WorkingTreeFilterModel;
    QVariant v = Config::self().get( QLatin1String("Worktree/Filters"),
                                     int( WTF_All ) );
    wtFilter->setSourceModel( mStatusModel );
    wtFilter->setFilter( WorkingTreeFilters( v.toInt() ) );
    //wtFilter->setDynamicSortFilter( true );

    WorkingTreeFilterModel *indexFilter = new WorkingTreeFilterModel;
    indexFilter->setSourceModel( mStatusModel );
    indexFilter->setFilter( WorkingTreeFilters( WTF_Missing | WTF_Changed ) );
    //wtFilter->setDynamicSortFilter( true );

    mTreeView->setModel( wtFilter );
    mIndexTreeView->setModel( indexFilter );

    mFilterRecursion = true;

    WorkingTreeFilters filters = wtFilter->filter();

    actShowAll->setChecked( filters == WTF_All );
    actShowIgnored->setChecked( filters & WTF_Ignored );
    actShowMissing->setChecked( filters & WTF_Missing );
    actShowModified->setChecked( filters & WTF_Changed );
    actShowUnchanged->setChecked( filters & WTF_Unchanged );
    actShowUntracked->setChecked( filters & WTF_Untracked );
    actShowStaged->setChecked( filters & WTF_Staged );

    mFilterRecursion = false;
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
    mRepo = repo;

    if( mRepo.isValid() )
    {
        updateDiff();
        mStatusModel->setRepository( repo );
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

void IndexWidget::setWtFilter(bool enabled, WorkingTreeFilter flag)
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilterModel *wtfModel = dynamic_cast<WorkingTreeFilterModel*>( mTreeView->model() );
    if ( wtfModel )
    {
        WorkingTreeFilters f = wtfModel->filter();
        wtfModel->setFilter( enabled ? f |= flag : f &= ~flag );
        Config::self().set( QLatin1String( "Worktree/Filters" ),
                            int( wtfModel->filter() ) );

        actShowAll->setChecked( f == WTF_All );
    }

    mFilterRecursion = false;
}

void IndexWidget::onShowAll( bool enabled )
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilterModel *wtfModel = dynamic_cast<WorkingTreeFilterModel*>( mTreeView->model() );
    if ( wtfModel )
        wtfModel->setFilter( enabled ? WTF_All : WTF_None );

    actShowIgnored->setChecked( enabled );
    actShowMissing->setChecked( enabled);
    actShowModified->setChecked( enabled );
    actShowUnchanged->setChecked( enabled );
    actShowUntracked->setChecked( enabled );
    actShowStaged->setChecked( enabled );

    mFilterRecursion = false;
}

void IndexWidget::onShowModified( bool enabled )
{
    setWtFilter( enabled, WTF_Changed );
}

void IndexWidget::onShowMissing( bool enabled )
{
    setWtFilter( enabled, WTF_Missing );
}

void IndexWidget::onShowIgnored( bool enabled )
{
    setWtFilter( enabled, WTF_Ignored );
}

void IndexWidget::onShowUntracked( bool enabled )
{
    setWtFilter( enabled, WTF_Untracked );
}

void IndexWidget::onShowUnchanged( bool enabled )
{
    setWtFilter( enabled, WTF_Unchanged );
}

void IndexWidget::onShowStaged( bool enabled )
{
    setWtFilter( enabled, WTF_Staged );
}

//void IndexWidget::workingTreeChanged()
//{
//    WorkingTreeModel* model = static_cast< WorkingTreeModel* >( mTreeView->model() );
//    if( !model )
//    {
//        return;
//    }

//    model->update();
//    updateDiff();
//}
