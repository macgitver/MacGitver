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

#include "libMacGitverCore/App/MacGitver.hpp"

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
}

/**
 * @brief       Menu action to fetch all remotes of a repository.
 */
void RemotesModule::onRemotesFetchAll()
{
    // TODO: requires a RepositoryContext (the repo to fetch all remotes from)
    // A sub-context (i.e. a branch) can further restrict, what is fetched
}

