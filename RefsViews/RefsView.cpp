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

#include <QListView>
#include <QVBoxLayout>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "RefsListModel.h"
#include "RefsView.h"

RefsView::RefsView()
    : View( QLatin1String( "Refs" ) )
{
    mListView = new QListView();
    mListView->setFrameStyle( QFrame::NoFrame );

    QVBoxLayout* l = new QVBoxLayout;
    l->setSpacing( 0 );
    l->setMargin( 0 );
    l->addWidget( mListView );

    setLayout( l );

    setViewName( trUtf8( "Refs" ) );

    mModel = new RefsListModel( this );
    mListView->setModel( mModel );

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void RefsView::repositoryChanged( Git::Repository repo )
{
    mRepo = repo;

    if( mRepo.isValid() )
    {
        Git::Result r;
        mModel->syncTo( mRepo.allReferences( r ) );
    }
    else
    {
        mModel->clear();
    }
}

QSize RefsView::sizeHint() const
{
    return QSize( 100, 100 );
}
