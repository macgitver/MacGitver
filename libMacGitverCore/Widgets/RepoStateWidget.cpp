/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include <QGridLayout>
#include <QLabel>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "MacGitver/RepoManager.hpp"
#include "MacGitver/RepositoryInfo.hpp"

#include "RepoStateWidget.hpp"

RepoStateWidget::RepoStateWidget()
{
    repoInfo = NULL;
    setupUi();

    connect( &MacGitver::repoMan(), SIGNAL(repositoryActivated(RepositoryInfo*)),
             this, SLOT(repositoryActivated(RepositoryInfo*)) );

    connect( &MacGitver::repoMan(), SIGNAL(repositoryDeactivated(RepositoryInfo*)),
             this, SLOT(repositoryDeactivated(RepositoryInfo*)) );

    //repositoryActivated( MacGitver::repoMan().activeRepository() );
}

void RepoStateWidget::repositoryActivated( RepositoryInfo* info )
{
    if( repoInfo != info )
    {
        repoInfo = info;
        setRepoState();
    }
}

void RepoStateWidget::repositoryDeactivated( RepositoryInfo* info )
{
    if( repoInfo == info )
    {
        repoInfo = NULL;
        setRepoState();
    }
}

void RepoStateWidget::setRepoState()
{
    txthlState->hide();
    txtState->hide();

    txtRepo->setText( repoInfo ? repoInfo->displayAlias() : QString() );
    txtBranch->setText( repoInfo ? repoInfo->branchDisplay() : QString() );
}

void RepoStateWidget::setupUi()
{
    txthlBranch = new QLabel( trUtf8( "Branch" ) );
    txthlState = new QLabel( trUtf8( "State" ) );
    txthlRepo = new QLabel( trUtf8( "Repository" ) );

    txtBranch = new QLabel;
    txtState = new QLabel;
    txtRepo = new QLabel;

    QGridLayout* l = new QGridLayout;
    l->setMargin( 0 );
    l->setSpacing( 1 );

    l->addWidget( txthlRepo, 0, 0 );
    l->addWidget( txtRepo, 1, 0 );

    l->addWidget( txthlState, 0, 1 );
    l->addWidget( txtState, 1, 1 );

    l->addWidget( txthlBranch, 0, 2 );
    l->addWidget( txtBranch, 1, 2 );

    setLayout( l );

    setRepoState();
}

