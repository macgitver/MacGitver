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

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"

#include "RepositoryModule.h"
#include "RepoTreeView.hpp"
#include "CloneRepositoryDlg.h"
#include "CreateRepositoryDlg.h"

RepositoryModule::RepositoryModule()
    : ConfigUser( "Repository" )
{
}

RepositoryModule::~RepositoryModule()
{
}

void RepositoryModule::repositoryChanged( Git::Repository newRepository )
{
    mRepo = newRepository;

    bool isValid = mRepo.isValid();

    actRepositoryClose->setEnabled( isValid );
}

void RepositoryModule::setupConfigPages( ConfigDialog* dialog )
{
}

Heaven::View* RepositoryModule::createRepoTreeView()
{
    return new RepoTreeView;
}

void RepositoryModule::initialize()
{
    setupActions( this );
    acRepositoryMenuAC->mergeInto( "RepositoryMenuMP" );
    acRepositoryToolBarAC->mergeInto( "RepositoryToolBarMP" );

    MacGitver::self().registerView( "RepoTree",
                                    tr( "Repository" ),
                                    &RepositoryModule::createRepoTreeView );
}

void RepositoryModule::deinitialize()
{
    MacGitver::self().unregisterView( "RepoTree" );
}

void RepositoryModule::onRepositoryClose()
{
    if( mRepo.isValid() )
    {
        MacGitver::self().setRepository( Git::Repository() );
    }
}

void RepositoryModule::onRepositoryCreate()
{
    CreateRepositoryDlg().exec();
}

void RepositoryModule::onRepositoryOpen()
{
    MacGitver::repoMan().open();
}

void RepositoryModule::onRepositoryClone()
{
    CloneRepositoryDlg().exec();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Repository, RepositoryModule )
#endif
