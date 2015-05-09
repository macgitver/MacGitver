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

#include <QStandardItemModel>
#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>

#include "libGitWrap/ObjectId.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/FSWatcher.h"

#include "SubmodulesView.h"
#include "SubmodulesDelegate.h"
#include "SubmodulesCreateEditDlg.h"


SubmodulesView::SubmodulesView()
    : View( "Submodules" )
{
    setupActions( this );

    setToolBar( tbSMViewToolbar );

    mTree = new QTreeView;
    mTree->setRootIsDecorated( false );
    mTree->setHeaderHidden( true );
    mTree->setFrameShape( QFrame::NoFrame );
    setWidget( mTree );

    mModel = new QStandardItemModel( this );
    mTree->setModel( mModel );

    mTree->setItemDelegate( new SubmodulesViewDelegate( this ) );

    setViewName( trUtf8( "Submodules" ) );

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }

//  connect( MacGitver::self().watcher(), SIGNAL(repoGitFileChanged()),
//           this, SLOT(readSubmodules()) );
}

void SubmodulesView::repositoryChanged( Git::Repository repo )
{
    mRepo = repo;
    readSubmodules();
}

void SubmodulesView::readSubmodules()
{
    if( !mRepo.isValid() )
        return;

    Git::Result r;
    QList<Git::Submodule> submodules( mRepo.submodules(r) );
    if (!r)
    {
        QMessageBox::critical( 0, trUtf8("Error while reading submodules")
                               , trUtf8("Error while reading submodules:\n%1").arg(r.errorText()) );
    }

    // TODO: use IconProvider
    QIcon decoration; // ( IconProvider::self().icon(QStringLiteral("subrepo")) );

    foreach( Git::Submodule module, submodules )
    {
        QStandardItem * it = new QStandardItem( decoration, trUtf8("<INVALID>") );
        it->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        if( module.isValid() )
            it->setData( QVariant::fromValue(module), Qt::UserRole + 1 );

        mModel->appendRow( it );
    }
}

void SubmodulesView::addSubmodule()
{
    SubmodulesCreateEditDlg d;
    d.exec();
}
