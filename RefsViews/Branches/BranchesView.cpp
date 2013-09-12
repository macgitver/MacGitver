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
#include "RefsSortProxy.hpp"


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
    QModelIndex srcIndex = mData->mSortProxy->deeplyMapToSource( index );
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

    QModelIndex srcIndex = mData->mSortProxy->deeplyMapToSource( mTree->currentIndex() );
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

    QModelIndex srcIndex = mData->mSortProxy->deeplyMapToSource( mTree->currentIndex() );
    if ( !srcIndex.isValid() ) return;

    const RefBranch *branch = static_cast<const RefBranch *>( srcIndex.internalPointer() );
    if ( !branch ) return;

    if ( !askToGoOn( trUtf8("Delete reference \'%1\'?").arg(branch->reference().shorthand()) ) )
        return;

    Git::Result r;
    Git::Reference ref = branch->reference();

    if ( !checkRemoveRef( ref ) ) return;

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

bool BranchesView::checkRemoveRef( const Git::Reference& ref )
{
    if ( !ref.isLocal() )
    {
        // TODO: Remove remote branches and tags by doing something like 'git push <REMOTE> :refspec'
        const QString tmpRefName = ref.shorthand();
        QString tmpSplitRefName = ref.isRemote() ?
                    ( tmpRefName.section(L'/', 0, 0) + QString::fromUtf8(" :") + tmpRefName.section(L'/', 1) )
                  : ( QString::fromUtf8( "<REMOTE> :" ) + tmpRefName );

        QMessageBox::information( this, trUtf8("Not implemented yet.")
                                  , trUtf8("Sorry, this feature is not available yet."
                                           " To delete the remote reference '%1', please type in a terminal:"
                                           "\n`$ git push %2`")
                                  .arg(tmpRefName, tmpSplitRefName ) );
        return false;
    }

    if ( ref.isCurrentBranch() )
    {
        bool goOn = askToGoOn( trUtf8( "You are about to delete the currently active branch '%1'." )
                               .arg( ref.shorthand() ) );
        if ( !goOn ) return false;
    }

    Git::Result r;

    // is there another reference pointing to here?
    bool foundBuddyRef = false;
    foreach ( const Git::Reference& current, ref.repository(r).allReferences(r) )
    {
        foundBuddyRef = ( current == ref ) && ( (current.name() != ref.name()) );
        if (foundBuddyRef)
            break;
    }

    if ( !foundBuddyRef )
    {
        // TODO: also check, if the commit is otherwise "visible"
        // -> Is it a parent of a referenced commit?
        // In this case the reference can safely be deleted.

        bool goOn = askToGoOn( trUtf8( "You are about to remove the last reference '%1'."
                                       "\nThis will move all commits in this branch to the \"lost-found\" area." )
                               .arg( ref.shorthand() ) );
        if ( !goOn ) return false;
    }

    return true;
}

void BranchesView::onRenameRef()
{
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    if ( !action ) return;

    QModelIndex proxyIndex = mTree->currentIndex();
    if ( !proxyIndex.isValid() ) return;

    // note: Resolving the proxy index is done by the QTreeView::edit().
    mTree->edit( proxyIndex );
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
        showContextMenu( mTree->indexAt( ev->pos() ), ev->globalPos() );
        ev->accept();
    }
}

void BranchesView::attachedToContext( Heaven::ViewContext* ctx, Heaven::ViewContextData* data )
{
    Q_UNUSED( ctx );

    BranchesViewData* myData = qobject_cast< BranchesViewData* >( data );
    Q_ASSERT( myData );

    mTree->setModel( NULL );
    delete mData;
    mData = myData;

    connect( mData->mModel, SIGNAL(gitError(const Git::Result&))
             , this, SLOT(actionFailed(const Git::Result&)) );

    mTree->setModel( mData->mSortProxy );
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
