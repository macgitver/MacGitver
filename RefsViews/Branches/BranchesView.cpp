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

#include <QContextMenuEvent>
#include <QMessageBox>
#include <QTreeView>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "Branches/BranchesView.hpp"
#include "Branches/BranchesModel.hpp"
#include "Branches/BranchesViewData.hpp"

#include "libGitWrap/Reference.hpp"

#include "RefItem.hpp"

BranchesView::BranchesView()
    : ContextView( "Branches" )
    , mData( NULL )
{
    mTree = new QTreeView;
#ifdef Q_OS_MACX
    mTree->setAttribute( Qt::WA_MacShowFocusRect, false );
#endif
    mTree->setFrameStyle( QFrame::NoFrame );
    mTree->setIndentation( 12 );
    mTree->setHeaderHidden( true );
    mTree->setRootIsDecorated( false );

    setupActions( this );

    setViewName( trUtf8( "References" ) );
    setToolBar( tbBranchesTB );
    setWidget( mTree );

    setFlags( ConsumesContexts | DataPerContext );
    setContextProvider( "RepoTree" );
}

Heaven::ViewContextData* BranchesView::createContextData() const
{
    return new BranchesViewData;
}

QSize BranchesView::sizeHint() const
{
    return QSize( 120, 100 );
}

void BranchesView::showContextMenu(const QModelIndex &index, const QPoint &globalPos)
{
    QModelIndex srcIndex = index;
    if ( !srcIndex.isValid() )
        return;

    const RefItem *item = static_cast<const RefItem*>(srcIndex.internalPointer());

    Heaven::Menu* menu = 0;
    if ( item && item->isContentItem() )
    {
        menu = menuCtxMenuRefsView;
        //menu->setActivationContext( item );
    }

    if ( menu )
        menu->showPopup( globalPos );
}

void BranchesView::onCheckoutRef()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action )
        return;

    QModelIndex srcIndex = mTree->currentIndex();
    if ( !srcIndex.isValid() )
        return;

    const RefBranch *branch = static_cast<const RefBranch *>( srcIndex.internalPointer() );
    if ( !branch ) return;

    Git::Result r;
    branch->reference().checkout(r);

    if ( !r )
    {
        QMessageBox::warning( this, trUtf8("Error while checking out reference."),
                              trUtf8("Failed to checkout reference (%1).\nGit message: %2")
                              .arg(branch->reference().shorthand())
                              .arg(r.errorText()) );
    }

    // TODO: workaround to update the views
    update();
}

void BranchesView::onRemoveRef()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action ) return;

    QModelIndex srcIndex = mTree->currentIndex();
    if ( !srcIndex.isValid() ) return;

    const RefBranch *branch = static_cast<const RefBranch *>( srcIndex.internalPointer() );
    if ( !branch ) return;

    if ( !askToGoOn( trUtf8("Delete reference \'%1\'?").arg(branch->reference().shorthand()) ) )
        return;

    Git::Result r;
    Git::Reference ref = branch->reference();
    // TODO: perform some safety checks before actually deleting the git-ref
    ref.destroy(r);

    if ( !r )
    {
        QMessageBox::warning( this, trUtf8("Error while removing reference."),
                              trUtf8("Failed to remove reference (%1).\nGit message: %2")
                              .arg(branch->reference().shorthand())
                              .arg(r.errorText()) );
    }

    // TODO: workaround to update the views
    mData->mModel->rereadBranches();
}

void BranchesView::onRenameRef()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action )
        return;

    QModelIndex srcIndex = mTree->currentIndex();
    if ( !srcIndex.isValid() )
        return;

    mTree->edit( srcIndex );
}

void BranchesView::actionFailed( const Git::Result& error )
{
    if ( error ) return;

    QMessageBox::warning( this, trUtf8("Error on last action."),
                          trUtf8("The action Failed!\nGit message: %1")
                          .arg(error.errorText()) );
}

void BranchesView::contextMenuEvent(QContextMenuEvent *ev)
{
    if( ev->reason() == QContextMenuEvent::Keyboard )
    {
        showContextMenu( QModelIndex(), ev->globalPos() );
        ev->accept();
    }
    else if( ev->reason() == QContextMenuEvent::Mouse )
    {
        QModelIndex idx = mTree->indexAt( ev->pos() );
        showContextMenu( idx, ev->globalPos() );
        ev->accept();
    }
}

void BranchesView::attachedToContext( Heaven::ViewContext* ctx, Heaven::ViewContextData* data )
{
    BranchesViewData* myData = qobject_cast< BranchesViewData* >( data );
    Q_ASSERT( myData );

    mTree->setModel( NULL );
    delete mData;
    mData = myData;

    connect( mData->mModel, SIGNAL(gitError(const Git::Result&))
             , this, SLOT(actionFailed(const Git::Result&)) );

    mTree->setModel( mData->mModel );
}

void BranchesView::detachedFromContext( Heaven::ViewContext* ctx )
{
    mTree->setModel( NULL );
    mData = NULL;
}

bool BranchesView::askToGoOn(const QString &message)
{
    QMessageBox::StandardButton answer =
            QMessageBox::question( this, trUtf8("Are you sure?"), message + trUtf8("\n\nAre you sure?")
                                   , QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
    return ( answer == QMessageBox::Yes );
}
