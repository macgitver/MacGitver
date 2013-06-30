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

BranchesView::BranchesView()
    : ContextView( "Branches" )
    , mData( NULL )
{
    mTree = new QTreeView;
    mTree->setFrameStyle( QFrame::NoFrame );
    mTree->setIndentation( 12 );
    mTree->setHeaderHidden( true );
    mTree->setRootIsDecorated( false );

    setupActions( this );

    setViewName( trUtf8( "Branches" ) );
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

    QString refName = index.data().toString();

    Heaven::Menu* menu = 0;
    if ( !refName.isEmpty() )
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

    const BranchesModel::Item *item =
            static_cast<const BranchesModel::Item *>( srcIndex.internalPointer() );
    Q_ASSERT( item );

    Git::Result r;
    // TODO: Do it right! The Reference object is used as a placeholder here!.
    Git::Reference ref = mData->repository().lookupRef(
                r, QString::fromUtf8("refs/heads/%1").arg(item->text) );

    ref.checkout(r);

    if ( !r )
    {
        QMessageBox::warning( this, trUtf8("Error while checking out reference."),
                              trUtf8("Failed to checkout reference (%1).\nGit message: %2")
                              .arg(item->text)
                              .arg(r.errorText()) );
    }
    else
    {
        // TODO: workaround to update the model
        mData->mModel->rereadBranches();
    }
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
    mData = myData;

    mTree->setModel( mData->mModel );
}

void BranchesView::detachedFromContext( Heaven::ViewContext* ctx )
{
    mTree->setModel( NULL );
    mData = NULL;
}
