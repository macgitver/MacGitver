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

#include <QtPlugin>
#include <QFileDialog>
#include <QMessageBox>

#include "libGitWrap/Operations/RemoteOperations.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include "RemoteCreateEditDlg.h"
#include "RemotesModule.h"
#include "RemotesView.h"

RemotesModule::RemotesModule()
{
}

BlueSky::View* RemotesModule::createRemotesView()
{
    return new RemotesView;
}

void RemotesModule::initialize()
{
    setupActions( this );

     acRemotesAC->mergeInto( "RemotesMP" );
     acRemotesFetchAC->mergeInto( "RemotesFetchMP" );

    MacGitver::self().registerView( "Remotes", tr( "Remotes" ),
                                    &RemotesModule::createRemotesView );
}

void RemotesModule::deinitialize()
{
    MacGitver::self().unregisterView( "Remotes" );
}

/**
 * @brief       Menu action to create a remote and add it to a repository.
 */
void RemotesModule::onRemoteCreateEdit()
{
    // TODO: requires a repository context (the repo to add the remote to)
    // To edit an existing remote, the remote context is required
    RemoteCreateEditDlg dlg;
    //TODO: dlg.setContext( ctx );
    dlg.exec();
}

void RemotesModule::onRemoteDelete()
{
    // TODO: requires the remote context (the remote to delete)
}

/**
 * @brief       Menu action to fetch all remotes of a repository.
 */
void RemotesModule::onRemotesFetchAll()
{
    // TODO: requires a repository context (the repo to fetch all remotes from)
    // A sub-context (i.e. a branch) can further restrict, what is fetched
    Heaven::Action* action = qobject_cast< Heaven::Action* >( sender() );
    Q_ASSERT( action );
    RM::Repo* repo = qobject_cast< RM::Repo* >( action->activatedBy() );
    if( repo ) {
        Git::Result r;
        Git::Repository gitRepo = repo->gitRepo();
        const QStringList aliases( gitRepo.allRemoteNames(r) );
        if ( !r ) {
            QMessageBox::warning( 0, tr("Lookup of remotes failed"),
                                  tr("Unable to lookup remotes for repository '%1'."
                                     "\nMessage: %2").arg(repo->displayName())
                                  .arg(r.errorText())
                                  );
            return;
        }

        if ( aliases.isEmpty() ) {
            QMessageBox::information( 0, tr("No Remotes found"),
                                      tr("No remotes configured for repository '%1'.")
                                      .arg(repo->displayName())
                                      );
            return;
        }

        foreach (const QString& alias, aliases) {
            Git::FetchOperation* op = new Git::FetchOperation( repo->gitRepo() );
            op->setRemoteAlias( alias );
            op->setBackgroundMode( true );
            connect( op, SIGNAL(finished()), this, SLOT(fetchOperationFinished()) );
            // TODO: create a central dialog to show progress of parallel operations
            op->execute();
        }
    }
}

/**
 * @brief       Called, when an non-blocking Git::Operation finished.
 */
void RemotesModule::onOperationFinished()
{
    Git::BaseOperation* op = qobject_cast<Git::BaseOperation*>( sender() );
    Q_ASSERT( op );
    Git::Result r( op->result() );
    if ( !r ) {
        QMessageBox::warning( 0, tr("Operation failed."),
                              tr("Operation failed. Message:\n %1").arg(r.errorText())
                              );
    }

    // delete the operation
    op->deleteLater();
}

