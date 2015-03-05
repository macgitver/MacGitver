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

#include <QStringBuilder>
#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QHeaderView>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Index.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/ObjectTree.hpp"
#include "libGitWrap/DiffList.hpp"

#include "libDiffViews/Views/Interfaces/DiffViews.hpp"
#include "libDiffViews/Views/Interfaces/DiffView.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Widgets/SHA1Input.h"
#include "libMacGitverCore/Widgets/ShortCommitModel.h"
#include "libMacGitverCore/Widgets/FlatTreeComboBox.h"
#include "libMacGitverCore/MacGitver/GitPatchConsumer.hpp"

#include "HistoryDiff.h"

HistoryDiff::HistoryDiff()
{
    mDiffView = DiffViews::DiffViews::self().defaultCreator()->create( this );

    QVBoxLayout* l = new QVBoxLayout;
    l->setMargin( 0 );
    l->setSpacing( 0 );
    setLayout( l );

    mToolbar = new QToolBar;

    mDiffTo = new QComboBox;
    mDiffTo->addItem( trUtf8( "Workingtree" ), DTT_WT );
    mDiffTo->addItem( trUtf8( "Index" ), DTT_Index );
    mDiffTo->addItem( trUtf8( "Workingtree + Index" ), DTT_WT_and_Index );
    mDiffTo->addItem( trUtf8( "HEAD" ), DTT_HEAD );
    //mDiffTo->addItem( trUtf8( "SHA-1" ), DTT_AnySHA1 );
    mDiffTo->addItem( trUtf8( "Branch" ), DTT_Branch );
    mDiffTo->addItem( trUtf8( "Tag" ), DTT_Tag );
    mDiffTo->addItem( trUtf8( "All parents" ), DTT_AllParents );
    mDiffTo->addItem( trUtf8( "Parent" ), DTT_Parent );

    mToolbar->addWidget( new QLabel( trUtf8( "Diff to:" ) ) );
    mToolbar->addWidget( mDiffTo );

    mDiffToBranch = new FlatTreeComboBox;
    mDiffToActions.insert( DTT_Branch, mToolbar->addWidget( mDiffToBranch ) );

    mDiffToTag = new FlatTreeComboBox;
    mDiffToActions.insert( DTT_Tag, mToolbar->addWidget( mDiffToTag ) );

    mDiffToParent = new QComboBox;

    mParentsModel = new ShortCommitModel( this );
    mDiffToParent->setModel( mParentsModel );
    mParentsList = new QTreeView;
    mParentsList->setHeaderHidden( true );
    mParentsList->setRootIsDecorated( false );

    #if QT_VERSION < 0x050000
    // QT5: Recheck this.
    mParentsList->header()->setResizeMode( QHeaderView::ResizeToContents );
    #endif

    mParentsList->setModel( mParentsModel );
    mDiffToParent->setView( mParentsList );

    mDiffToActions.insert( DTT_Parent, mToolbar->addWidget( mDiffToParent ) );

    mDiffToSha1 = new SHA1Input;
    mDiffToActions.insert( DTT_AnySHA1, mToolbar->addWidget( mDiffToSha1 ) );

    foreach( QAction* act, mDiffToActions )
    {
        act->setVisible( false );
    }

    connect( mDiffTo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onDiffToChanged(int)) );

    connect( mDiffToParent, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    connect( mDiffToBranch, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    connect( mDiffToTag, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    l->addWidget( mToolbar );
    l->addWidget( mDiffView );
    setLayout( l );
}

void HistoryDiff::setRepository( Git::Repository repo )
{
    Git::Result r;

    mRepo = repo;

    if( mRepo.isValid() )
    {

        QStringList list = mRepo.allBranchNames( r );
        if( r )
        {
            mDiffToBranch->clear();
            mDiffToBranch->add( list );
        }
        else
        {
            r.clear();
        }

        list = mRepo.allTagNames( r );
        if( r )
        {
            mDiffToTag->clear();
            mDiffToTag->add( list );
        }
        else
        {
            r.clear();
        }

    }

}

void HistoryDiff::setCommitId( const Git::ObjectId& sha1 )
{
    setPatch( DiffViews::Patch::Ptr() );

    if( sha1.isNull() )
    {
        return;
    }

    Git::Result r;

    mCommit = mRepo.lookupCommit( r, sha1 );
    mRepo.index( r ).read( r );

    mParentsModel->setCommitList( mCommit.parentCommits( r ) );

    if( !r )
    {
        MacGitver::log(Log::Error, r);
        return;
    }

    createPatch();
}

void HistoryDiff::onDiffToChanged( int index )
{
    if( index == -1 )
    {
        setPatch( DiffViews::Patch::Ptr() );
        return;
    }

    DiffToTypes dtt = DiffToTypes( mDiffTo->itemData( index ).toInt() );
    foreach( DiffToTypes type, mDiffToActions.keys() )
    {
        mDiffToActions[ type ]->setVisible( dtt == type );
    }

    createPatch();
}

Git::DiffList HistoryDiff::makePatchTo( const QString& ref )
{
    Git::Result r;
    Git::ObjectId oid = mRepo.reference(r, ref).resolveToObjectId( r );
    if( !r )
    {
        return Git::DiffList();
    }

    Git::Commit commit = mRepo.lookupCommit( r, oid );
    if( !r )
    {
        return Git::DiffList();
    }

    return mCommit.diffFrom( r, commit );
}

void HistoryDiff::createPatch()
{
    if( !mCommit.isValid() )
    {
        return;
    }

    int index = mDiffTo->currentIndex();
    DiffToTypes dtt = DiffToTypes( mDiffTo->itemData( index ).toInt() );
    Git::Result r;
    Git::DiffList dl;

    switch( dtt )
    {
    case DTT_WT:
        dl = Git::Diff( r ).treeToWorkDir( r, mCommit.tree( r ) );
        break;

    case DTT_WT_and_Index:
        dl = Git::Diff( r ).treeToWorkDirWithIndex( r, mCommit.tree( r ) );
        break;

    case DTT_AllParents:
        dl = mCommit.diffFromAllParents( r );
        break;

    case DTT_Parent:
        index = mDiffToParent->currentIndex();
        if( index != -1 )
        {
            dl = mCommit.diffFromParent( r, (unsigned int) index );
        }
        break;

    case DTT_HEAD:
        dl = makePatchTo( QStringLiteral( "HEAD" ) );
        break;

    case DTT_Branch:
        dl = makePatchTo( QStringLiteral( "refs/heads/" ) % mDiffToBranch->currentText() );
        break;

    case DTT_Tag:
        dl = makePatchTo( QStringLiteral( "refs/tags/" ) % mDiffToTag->currentText() );
        break;

    default:
        break;
    }

    if( dl.isValid() )
    {
        GitPatchConsumer p;
        dl.findRenames( r );
        dl.consumePatch( r, &p );
        setPatch( p.patch() );
    }
    else
    {
        setPatch( DiffViews::Patch::Ptr() );
    }
}

void HistoryDiff::setPatch( DiffViews::Patch::Ptr patch )
{
    mDiffView->setPatch( patch );
}
