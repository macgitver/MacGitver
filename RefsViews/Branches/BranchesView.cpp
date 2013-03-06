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

#include <QTreeView>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "Branches/BranchesView.hpp"
#include "Branches/BranchesModel.hpp"
#include "Branches/BranchesViewData.hpp"

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

void BranchesView::attachedContext( Heaven::ViewContext* ctx, Heaven::ViewContextData* data )
{
    BranchesViewData* myData = qobject_cast< BranchesViewData* >( data );
    Q_ASSERT( myData );
    mData = myData;

    mTree->setModel( mData->mModel );
}

void BranchesView::detachedContext( Heaven::ViewContext* ctx )
{
    mTree->setModel( NULL );
    mData = NULL;
}
