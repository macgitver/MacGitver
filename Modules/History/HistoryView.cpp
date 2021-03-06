/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Commit.hpp"

#include "libDiffViews/Views/Raw/RawView.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include "HistoryView.h"
#include "HistoryEntry.h"
#include "HistoryModel.h"
#include "HistoryDiff.h"
#include "HistoryList.h"
#include "HistoryListDelegate.h"
#include "HistoryDetails.h"

HistoryView::HistoryView()
    : View( "History" )
    , ConfigUser( "History" )
    , mModel( 0 )
    , mRepo( 0 )
{
    setupActions( this );

    mVertSplit = mHorzSplit = NULL;

    setViewName( trUtf8( "History" ) );
    setToolBar( tbHistoryViewToolBar );

    mList = new HistoryList;
    mList->setFrameShape( QFrame::NoFrame );

    connect( mList, SIGNAL(currentCommitChanged(Git::ObjectId)),
             this, SLOT(currentCommitChanged(Git::ObjectId)) );

    mList->setItemDelegate( new HistoryListDelegate );

    mDetails = new HistoryDetails;
    mDiff = new HistoryDiff;

    actHistoryShowHEADonly->setChecked(true);

    initSplitters();

    connect( &MacGitver::repoMan(), &RM::RepoMan::repositoryActivated,
             this,                  &HistoryView::repoActivated);
}

void HistoryView::repoActivated(RM::Repo* repo)
{
    if (mRepo != repo) {

        mRepo = repo;

        Git::Repository repo = mRepo->gitRepo();

        if (mModel) {
            mList->setModel(NULL);
            mModel->deleteLater();
        }

        mDetails->setRepository(repo);
        mDiff->setRepository(repo);

        if (repo.isValid()) {
            mModel = new HistoryModel(repo, this);

            mModel->changeDisplays(HistoryModel::DisplayLocals,
                                   actHistoryViewShowLocalBranches->isChecked());

            mModel->changeDisplays(HistoryModel::DisplayRemotes,
                                   actHistoryViewShowRemoteBranches->isChecked());

            mModel->changeDisplays(HistoryModel::DisplayTags,
                                   actHistoryViewShowTags->isChecked());

            mModel->buildHistory();
            mList->setModel(mModel);
        }
    }
}

void HistoryView::currentCommitChanged(const Git::ObjectId& sha1)
{
    mDetails->setCommit( sha1 );
    mDiff->setCommitId( sha1 );
}

void HistoryView::configChanged( const QString& subPath, const QVariant& value )
{
    if( subPath == QStringLiteral( "SplitLayout" ) )
    {
        initSplitters();
    }
}

void HistoryView::initSplitters()
{
    if( !mVertSplit )
    {
        mVertSplit = new BlueSky::MiniSplitter( Qt::Vertical );
    }
    else
    {
        mVertSplit->hide();
        mVertSplit->setParent( NULL );
    }

    if( !mHorzSplit )
    {
        mHorzSplit = new BlueSky::MiniSplitter( Qt::Horizontal );
    }
    else
    {
        mHorzSplit->hide();
        mHorzSplit->setParent( NULL );
    }

    mDiff->hide();
    mDiff->setParent( NULL );

    mList->hide();
    mList->setParent( NULL );

    mDetails->hide();
    mDetails->setParent( NULL );

    int i = configGet( "SplitLayout", 0 );
    switch( i )
    {
    default:
    case 0:
        mHorzSplit->addWidget( mDetails );
        mHorzSplit->addWidget( mDiff );
        mVertSplit->addWidget( mList );
        mVertSplit->addWidget( mHorzSplit );
        setWidget( mVertSplit );
        break;
    case 1:
        mVertSplit->addWidget( mDetails );
        mVertSplit->addWidget( mDiff );
        mHorzSplit->addWidget( mList );
        mHorzSplit->addWidget( mVertSplit );
        setWidget( mHorzSplit );
        break;
    case 2:
        mVertSplit->addWidget( mList );
        mVertSplit->addWidget( mDetails );
        mHorzSplit->addWidget( mVertSplit );
        mHorzSplit->addWidget( mDiff );
        setWidget( mHorzSplit );
        break;
    }

    mVertSplit->show();
    mHorzSplit->show();
    mDiff->show();
    mList->show();
    mDetails->show();
}

void HistoryView::onChangeShowLocalBranches(bool checked)
{
    if (checked) {
        setShowBranches(HistoryModel::ShowRootLocalBranches);
    }
}

void HistoryView::onChangeShowAllBranches(bool checked)
{
    if (checked) {
        setShowBranches(HistoryModel::ShowRootAllBranches);
    }
}

void HistoryView::onChangeShowHeadOnly(bool checked)
{
    if (checked) {
        setShowBranches(HistoryModel::ShowRootHeadOnly);
    }
}

void HistoryView::onChangeShowLocal(bool checked)
{
    if (mModel) {
        mModel->changeDisplays(HistoryModel::DisplayLocals, checked);
    }
}

void HistoryView::onChangeShowAllRefs(bool checked)
{
    if (checked) {
        setShowBranches(HistoryModel::ShowRootAllRefs);
    }
}

void HistoryView::onChangeShowRemote(bool checked)
{
    if (mModel) {
        mModel->changeDisplays(HistoryModel::DisplayRemotes, checked);
    }
}

void HistoryView::onChangeShowTags(bool checked)
{
    if (mModel) {
        mModel->changeDisplays(HistoryModel::DisplayTags, checked);
    }
}

void HistoryView::setShowBranches(HistoryModel::Roots branches)
{
    if (mModel) {
        mModel->setShowRoots(branches);
    }
}
