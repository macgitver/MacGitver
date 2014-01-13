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

#include "libHeavenIcons/IconRef.hpp"
#include "libHeavenIcons/Icon.hpp"

#include "RepoMan/RepoMan.hpp"
#include "RepoMan/Repo.hpp"

#include "RepoStateWidget.hpp"

RepoStateWidget::RepoStateWidget()
{
    repo = NULL;
    setupUi();

    connect( &MacGitver::repoMan(), SIGNAL(repositoryActivated(RM::Repo*)),
             this,                  SLOT(repositoryActivated(RM::Repo*)) );

    connect( &MacGitver::repoMan(), SIGNAL(repositoryDeactivated(RM::Repo*)),
             this,                  SLOT(repositoryDeactivated(RM::Repo*)) );
}

void RepoStateWidget::repositoryActivated(RM::Repo* info)
{
    if( repo != info )
    {
        repo = info;
        setRepoState();
    }
}

void RepoStateWidget::repositoryDeactivated(RM::Repo* info)
{
    if( repo == info )
    {
        repo = NULL;
        setRepoState();
    }
}

void RepoStateWidget::setRepoState()
{
    txtState->hide();

    txtRepo->setText( repo ? repo->displayAlias() : QString() );
    txtBranch->setText( repo ? repo->branchDisplay() : QString() );
}

static inline QLabel* pixmapLabel(const char* name)
{
    Heaven::IconRef iconRef(name, 16);
    QLabel* l = new QLabel;
    l->setPixmap(iconRef.icon().pixmap());
    return l;
}

void RepoStateWidget::setupUi()
{
    txtBranch = new QLabel;
    txtState = new QLabel;
    txtRepo = new QLabel;

    QHBoxLayout* l = new QHBoxLayout;
    l->setMargin(0);
    l->setSpacing(3);

    l->addWidget(pixmapLabel("Repo"));
    l->addWidget(txtRepo);

    l->addWidget(txtState);

    l->addWidget(pixmapLabel("Branch"));
    l->addWidget(txtBranch);

    setLayout( l );

    setRepoState();
}

