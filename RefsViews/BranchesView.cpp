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

#include "BranchesView.hpp"
#include "BranchesModel.hpp"

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

    setContextProvider( QLatin1String( "" ) );

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void BranchesView::repositoryChanged( Git::Repository repo )
{
    if( !mData )
    {
        mData = new BranchesViewData( this );
        mData->mModel = new BranchesModel( mData );
        mTree->setModel( mData->mModel );
    }

    mData->mRepo = repo;
    mData->mModel->setRepository( repo );
    mData->mModel->rereadBranches();
    mTree->expandAll();
}

QSize BranchesView::sizeHint() const
{
    return QSize( 100, 100 );
}
