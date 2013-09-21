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
#include "libMacGitverCore/MacGitver/GitPatchConsumer.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"
#include "libMacGitverCore/MacGitver/RepositoryInfo.hpp"

#include "IndexTreeItemView.h"
#include "WorkingTreeItemView.h"
#include "WorkingTreeModel.h"
#include "WorkingTreeFilterModel.h"

IndexWidget::IndexWidget()
    : View( "WorkTree" )
    , mSplitter( new Heaven::MiniSplitter( Qt::Vertical ) )
    , mTreeView( new WorkingTreeItemView )
    , mIndexTreeView( new IndexTreeItemView )
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

    connect(&MacGitver::repoMan(), SIGNAL(repositoryActivated(RepositoryInfo*)),
            this, SLOT(repositoryActivated(RepositoryInfo*)));

    connect(&MacGitver::repoMan(), SIGNAL(repositoryDeactivated(RepositoryInfo*)),
            this, SLOT(repositoryDeactivated(RepositoryInfo*)));
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

void IndexWidget::repositoryActivated(RepositoryInfo* repo)
{
    if (mRepo != repo) {
        mRepo = repo;

        mStatusModel->setRepository(mRepo->gitRepo());

        updateDiff();
    }
}

void IndexWidget::repositoryDeactivated(RepositoryInfo* repo)
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
