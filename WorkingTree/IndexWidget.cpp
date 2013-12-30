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

#include "IndexWidget.h"

#include "libGitWrap/Index.hpp"
#include "libGitWrap/DiffList.hpp"

#include "libDiffViews/Model/FilePatch.hpp"
#include "libDiffViews/Views/Interfaces/DiffView.hpp"
#include "libDiffViews/Views/Interfaces/DiffViews.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/MacGitver/GitPatchConsumer.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include "CommitDialog.h"
#include "IndexTreeItemView.h"
#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"
#include "WorkingTreeFilterModel.h"

#include <QMessageBox>

IndexWidget::IndexWidget()
    : View( "WorkTree" )
    , mSplitter( new BlueSky::MiniSplitter( Qt::Vertical ) )
    , mTreeView( new WorkingTreeItemView )
    , mIndexTreeView( new IndexTreeItemView )
    , mCommitDialog( new CommitDialog )
    , mStatusModel( new WorkingTreeModel( this ) )
{
    mTreeView->setFrameShape( QFrame::NoFrame );
    mIndexTreeView->setFrameShape( QFrame::NoFrame );

    setupActions( this );

    setToolBar( tbWorkingTree );
    setViewName( trUtf8( "Working tree" ) );

    mDiffView = DiffViews::DiffViews::self().defaultCreator()->create( this );

    BlueSky::MiniSplitter *hSplit_1 = new BlueSky::MiniSplitter( Qt::Horizontal );
    hSplit_1->addWidget( mTreeView );
    hSplit_1->addWidget( mDiffView );

    mSplitter->addWidget( hSplit_1 );

    BlueSky::MiniSplitter *hSplit_2 = new BlueSky::MiniSplitter( Qt::Horizontal );

    hSplit_2->addWidget( mIndexTreeView );
    hSplit_2->addWidget( mCommitDialog );

    mSplitter->addWidget( hSplit_2 );

    setWidget( mSplitter );

    setupFilters();

    connect(&MacGitver::repoMan(),  SIGNAL(repositoryActivated(RM::Repo*)),
            this,                   SLOT(repositoryActivated(RM::Repo*)));

    connect(&MacGitver::repoMan(),  SIGNAL(repositoryDeactivated(RM::Repo*)),
            this,                   SLOT(repositoryDeactivated(RM::Repo*)));

    connect(mCommitDialog, SIGNAL(aboutToCommit()), this, SLOT(onCommitChanges()));
}

void IndexWidget::updateWtFilterView(const WorkingTreeFilterModel * const wtFilter)
{
    mFilterRecursion = true;

    Git::StatusFlags filters = wtFilter->filter();

    actShowIgnored->setChecked( filters & Git::FileIgnored );
    actShowMissing->setChecked( filters & Git::FileWorkingTreeDeleted );
    actShowModified->setChecked( filters & Git::FileWorkingTreeModified );
    actShowUnchanged->setChecked( filters & Git::FileUnchanged );
    actShowUntracked->setChecked( filters & Git::FileWorkingTreeNew );

    mFilterRecursion = false;
}

void IndexWidget::setupFilters()
{
    WorkingTreeFilterModel *wtFilter = new WorkingTreeFilterModel;
    QVariant v = Config::self().get( QLatin1String("Worktree/Filters"),
                                     int( ALL_FILE_STATUS_FILTERS ) );
    wtFilter->setSourceModel( mStatusModel );
    wtFilter->setFilter( Git::StatusFlags( v.toInt() ) );
    //wtFilter->setDynamicSortFilter( true );

    WorkingTreeFilterModel *indexFilter = new WorkingTreeFilterModel;
    indexFilter->setSourceModel( mStatusModel );
    indexFilter->setFilter( Git::StatusFlags( Git::FileIndexNew
                                              | Git::FileIndexDeleted
                                              | Git::FileIndexModified ) );
    //wtFilter->setDynamicSortFilter( true );

    mTreeView->setModel( wtFilter );
    mIndexTreeView->setModel( indexFilter );

    updateWtFilterView( wtFilter );
}

void IndexWidget::repositoryActivated(RM::Repo* repo)
{
    if (mRepo != repo) {
        mRepo = repo;

        mStatusModel->setRepository(mRepo->gitRepo());

        updateDiff();
    }
}

void IndexWidget::repositoryDeactivated(RM::Repo* repo)
{
    if (mRepo == repo) {
        mRepo = NULL;

        mStatusModel->setRepository(Git::Repository());

        updateDiff();
    }
}

void IndexWidget::updateDiff()
{
    GitPatchConsumer p;

    if (mRepo) {
        Git::Result r;
        Git::DiffList dl = mRepo->gitRepo().diffIndexToWorkingDir(r);
        dl.consumePatch(r, &p);
    }

    mDiffView->setPatch(p.patch());
}

void IndexWidget::setWtFilter(bool enabled, Git::Status flag)
{
    if( mFilterRecursion )
        return;

    mFilterRecursion = true;

    WorkingTreeFilterModel *wtfModel = dynamic_cast<WorkingTreeFilterModel*>( mTreeView->model() );
    if ( wtfModel )
    {
        Git::StatusFlags f = wtfModel->filter();
        wtfModel->setFilter( enabled ? f |= flag : f &= ~flag );
        Config::self().set( QLatin1String( "Worktree/Filters" ),
                            int( wtfModel->filter() ) );
    }

    mFilterRecursion = false;
}

void IndexWidget::onShowAll()
{
    if( mFilterRecursion )
        return;

    WorkingTreeFilterModel *wtfModel = dynamic_cast<WorkingTreeFilterModel*>( mTreeView->model() );
    if ( wtfModel )
        wtfModel->setFilter( ALL_FILE_STATUS_FILTERS );

    updateWtFilterView( wtfModel );
}

void IndexWidget::onHideAll()
{
    if( mFilterRecursion )
        return;

    WorkingTreeFilterModel *wtfModel = dynamic_cast<WorkingTreeFilterModel*>( mTreeView->model() );
    if ( wtfModel )
        wtfModel->setFilter( Git::FileInvalidStatus );

    updateWtFilterView( wtfModel );
}

void IndexWidget::onShowModified( bool enabled )
{
    setWtFilter( enabled, Git::FileWorkingTreeModified );
}

void IndexWidget::onShowMissing( bool enabled )
{
    setWtFilter( enabled, Git::FileWorkingTreeDeleted );
}

void IndexWidget::onShowIgnored( bool enabled )
{
    setWtFilter( enabled, Git::FileIgnored );
}

void IndexWidget::onShowUntracked( bool enabled )
{
    setWtFilter( enabled, Git::FileWorkingTreeNew );
}

void IndexWidget::onShowUnchanged( bool enabled )
{
    setWtFilter( enabled, Git::FileUnchanged );
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
