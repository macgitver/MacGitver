/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
 * (C) Cunz RaD Ltd.
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

#include "libRepoMan/Frontend/Reference.hpp"
#include "libRepoMan/Frontend/Repo.hpp"

#include "libRepoMan/RepoMan.hpp"

#include "RepoStateWidget.hpp"

RepoStateWidget::RepoStateWidget()
{
    setupUi();

    RM::RepoMan& rm = RM::RepoMan::instance();

    connect(&rm,    &RM::RepoMan::refMoved,
            this,   &RepoStateWidget::onUpdateHEAD);

    connect(&rm,    &RM::RepoMan::repositoryActivated,
            this,   &RepoStateWidget::repositoryActivated);

    connect(&rm,    &RM::RepoMan::repositoryDeactivated,
            this,   &RepoStateWidget::repositoryDeactivated);
}

void RepoStateWidget::repositoryActivated(const RM::Frontend::Repo& repo)
{
    if (mRepo != repo) {
        mRepo = repo;
        setRepoState();
    }
}

void RepoStateWidget::repositoryDeactivated(const RM::Frontend::Repo& repo)
{
    if (mRepo == repo) {
        mRepo = RM::Frontend::Repo();
        setRepoState();
    }
}

void RepoStateWidget::setRepoState()
{
    txtState->hide();

    txtRepo->setText(mRepo ? mRepo.displayAlias() : QString());
    onUpdateHEAD(RM::Frontend::Reference());
}

void RepoStateWidget::onUpdateHEAD(
        const RM::Frontend::Reference& ref)
{
    RM::Frontend::Repo repo = ref.repository();

    if (repo != mRepo) {
        return;
    }

    if (ref && ref.name() != QStringLiteral("HEAD")) {
        return;
    }

    // TODO: implement a HTML formatter to highlight HEAD
    txtBranch->setText(repo ? repo.branchDisplay() : QString());
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

