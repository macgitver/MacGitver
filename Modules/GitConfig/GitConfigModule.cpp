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
#include "libRepoMan/RepoMan.hpp"

#include "GitConfigModule.h"
#include "GitConfigDialog.h"

GitConfigModule::GitConfigModule()
{
}

void GitConfigModule::initialize()
{
    setupActions( this );
    acGitConfigAC->mergeInto( "ToolsMenuMP" );
}

void GitConfigModule::deinitialize()
{
}

void GitConfigModule::onToolsGitConfig()
{
    RM::Frontend::Repo repo(RM::RepoMan::instance().activeRepository());

    #if 0 // ###REPOMAN
    Git::Repository gitRepo;
    if (repo) {
        gitRepo = repo->gitRepo();
    }
    GitConfigDialog( gitRepo ).exec();
    #endif
}
