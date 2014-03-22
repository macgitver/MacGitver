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

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"
#include "libMacGitverCore/Widgets/HeaderView.h"

#include "CreateBranchDialog.h"
#include "CreateTagDialog.h"
#include "HistoryList.h"
#include "HistoryModel.h"
#include "HistoryEntry.h"

#include "libGitWrap/Commit.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/Reference.hpp"

#include "libGitWrap/Operations/CheckoutOperation.hpp"

#include <QAbstractProxyModel>
#include <QMessageBox>

HistoryList::HistoryList( QWidget* parent )
    : TreeViewCtxMenu( parent )
    , ConfigUser( "History" )
{
    setRootIsDecorated( false );

    #ifdef Q_OS_MACX
    setAttribute( Qt::WA_MacShowFocusRect, false );
    #endif

    HeaderView* hv = new HeaderView( Qt::Horizontal );
    hv->setConfigName( configSubPath( "Columns" ) );
    setHeader( hv );

    setupActions( this );

    connect( this, SIGNAL(contextMenu(QModelIndex, QPoint)),
             this, SLOT(contextMenu(QModelIndex, QPoint)) );
}

void HistoryList::onCurrentChanged()
{
    if( currentIndex().isValid() )
    {
        const HistoryModel* hm = qobject_cast< const HistoryModel* >( currentIndex().model() );
        Q_ASSERT( hm );

        HistoryEntry* e = hm->indexToEntry( currentIndex() );
        if( e )
        {
            emit currentCommitChanged( e->id() );
            return;
        }
    }

    emit currentCommitChanged( Git::ObjectId() );
}

void HistoryList::setModel( QAbstractItemModel* model )
{
    TreeViewCtxMenu::setModel( model );
    mModel = qobject_cast< HistoryModel * >( model );

    connect( selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             this, SLOT(onCurrentChanged()) );

    // This call fixes the disappearing HeaderView.
    updateGeometries();
}

void HistoryList::configChanged( const QString& subPath, const QVariant& value )
{
    //qDebug() << "ConfigChange:" << subPath << "=>" << value;
}

void HistoryList::contextMenu( const QModelIndex& index, const QPoint& globalPos )
{
    Q_ASSERT( mModel );

    QModelIndex srcIndex = deeplyMapToSource( index );
    if ( !srcIndex.isValid() )
        return;

    HistoryEntry* item = mModel->indexToEntry( srcIndex );

    Heaven::Menu* menu = 0;
    if ( item )
    {
        menu = menuCtxMenuHistory;
        //menu->setActivationContext( item );
    }

    if ( menu )
        menu->showPopup( globalPos );
}

QModelIndex HistoryList::deeplyMapToSource( QModelIndex current ) const
{
    while( current.isValid() )
    {
        const QAbstractProxyModel* apm = qobject_cast< const QAbstractProxyModel* >( current.model() );
        if( !apm )
            return current;

        current = apm->mapToSource( current );
    }

    return QModelIndex();
}

void HistoryList::onCheckout()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action )
        return;

    QModelIndex srcIndex = deeplyMapToSource( currentIndex() );
    if ( !srcIndex.isValid() )
        return;

    HistoryEntry* item = mModel->indexToEntry( srcIndex );
    Q_ASSERT( item );

    Git::Result r;

    RM::Repo *repo = MacGitver::repoMan().activeRepository();
    Q_ASSERT( repo );

    Git::Repository gitRepo = repo->gitRepo();
    gitRepo.lookupCommit( r, item->id() ).checkout( r );

    if ( !r )
    {
        QMessageBox::information( this, trUtf8("Failed to checkout"),
                                  trUtf8("Failed to checkout. Git message:\n%1").arg(r.errorText()));
    }
}

void HistoryList::checkoutBranch(Git::Reference branch)
{
    Git::Result r;

    branch.checkoutOperation( r );

    if ( !r )
    {
        QMessageBox::information( this, trUtf8("Failed to create branch"),
                                  trUtf8("Failed to checkout branch '%1'. Git message:\n%2")
                                  .arg( branch.shorthand() )
                                  .arg( r.errorText() ) );
    }
}

void HistoryList::onCreateBranch()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action )
        return;

    QModelIndex srcIndex = deeplyMapToSource( currentIndex() );
    if ( !srcIndex.isValid() )
        return;

    HistoryEntry* item = mModel->indexToEntry( srcIndex );
    Q_ASSERT( item );

    Git::Result r;

    RM::Repo *repo = MacGitver::repoMan().activeRepository();
    Q_ASSERT( repo );

    CreateBranchDialog *dlg = new CreateBranchDialog(this);
    if ( dlg->exec() != QDialog::Accepted )
        return;

    Git::Repository gitRepo = repo->gitRepo();
    Git::Reference branch = gitRepo.lookupCommit( r, item->id() ).createBranch( r, dlg->branchName(), false );

    if ( !r )
    {
        QMessageBox::information( this, trUtf8("Failed to create branch"),
                                  trUtf8("Failed to create branch. Git message:\n%1").arg(r.errorText()) );
        return;
    }

    if ( dlg->checkoutBranch() )
    {
        checkoutBranch( branch );
    }
}



void HistoryList::onCreateTag()
{
    QMessageBox::information( this, trUtf8("Tag was not created"),
                              trUtf8("Creating tags is not available yet.") );

    return;

    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action )
        return;

    QModelIndex srcIndex = deeplyMapToSource( currentIndex() );
    if ( !srcIndex.isValid() )
        return;

    HistoryEntry* item = mModel->indexToEntry( srcIndex );
    Q_ASSERT( item );

    Git::Result r;

    RM::Repo *repo = MacGitver::repoMan().activeRepository();
    Q_ASSERT( repo );

    CreateTagDialog *dlg = new CreateTagDialog(this);
    if ( dlg->exec() != QDialog::Accepted )
        return;

    Git::Repository gitRepo = repo->gitRepo();
    // TODO: implementation of createTag() in libgGitWrap
//    Git::Tag tag = gitRepo.lookupCommit( r, item->id() ).createTag( r, dlg->tagName() );

    if ( !r )
    {
        QMessageBox::information( this, trUtf8("Failed to create tag"),
                                  trUtf8("Failed to create tag. Git message:\n%1").arg(r.errorText()) );
        return;
    }
}
