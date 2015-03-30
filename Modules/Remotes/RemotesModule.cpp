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
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "RemoteCreateEditDlg.h"
#include "RemotesModule.h"

RemotesModule::RemotesModule()
{
}

void RemotesModule::initialize()
{
    setupActions( this );

    acRemotesAC->mergeInto( "RemotesMP" );
    acFetchAC->mergeInto( "FetchMP" );
    acPushAC->mergeInto( "PushMP" );
}

void RemotesModule::deinitialize()
{
    MacGitver::self().unregisterView( "Remotes" );
}

/**
 * @brief       Menu action to create a remote and add it to a repository.
 */
void RemotesModule::onManageRemotes()
{
    Heaven::Action* a = qobject_cast<Heaven::Action*>( sender() );
    Q_ASSERT(a);

    // TODO: reliably determine the repository context
    QObject* actionCtx = a->activatedBy();
    RM::Repo* repoCtx = actionCtx ? static_cast<RM::Repo*>( actionCtx )
                                  : MacGitver::repoMan().activeRepository();

    RemoteCreateEditDlg( repoCtx ).exec();
}

/**
 * @brief       Menu action to fetch all remotes of a repository.
 */
void RemotesModule::onFetchAllRemotes()
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
            connect( op, &Git::FetchOperation::finished,
                     this, &RemotesModule::onOperationFinished );
            // TODO: create a central dialog to show progress of parallel operations
            op->execute();
        }
    }
}

void RemotesModule::onPushToAllRemotes()
{
    // TODO: implementation
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

