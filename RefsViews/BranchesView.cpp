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

#include <QListWidget>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "BranchesView.h"

BranchesView::BranchesView()
    : View( QLatin1String( "Branches" ) )
{
    mListWidget = new QListWidget();
    mListWidget->setFrameStyle( QFrame::NoFrame );

    setupActions( this );

    setViewName( trUtf8( "Branches" ) );
    setToolBar( tbBranchesTB );
    setWidget( mListWidget );

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
    mRepo = repo;
    rereadBranches();
}

void BranchesView::rereadBranches()
{
    mListWidget->clear();

    if( mRepo.isValid() )
    {
        Git::Result r;
        QString curBranch = mRepo.currentBranch( r );
        QStringList sl = mRepo.branches( actShowLocal->isChecked(), actShowRemote->isChecked(), r );

        for( int i = 0; i < sl.count(); i++ )
        {
            QListWidgetItem* it = new QListWidgetItem( sl[ i ], mListWidget );
            if( sl[ i ] == curBranch )
            {
                QFont f = it->font();
                f.setBold( true );
                it->setFont( f );
            }
        }

    }
}

QSize BranchesView::sizeHint() const
{
    return QSize( 100, 100 );
}
