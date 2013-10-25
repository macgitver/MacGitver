/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#include "RepoManConfigPage.hpp"

IMPLEMENT_NESTED_PAGE_CREATOR(RepoManConfigPage, 210)

RepoManConfigPage::RepoManConfigPage(ConfigDialog* dlg)
    : ConfigPage(dlg)
{
    setupUi(this);

    connect(spnGitRefresh, SIGNAL(valueChanged(int)), this, SLOT(setModified()));
    connect(spnIndexRefresh, SIGNAL(valueChanged(int)), this, SLOT(setModified()));
    connect(spnWorktreeRefresh, SIGNAL(valueChanged(int)), this, SLOT(setModified()));

    connect(chkGitRefresh, SIGNAL(clicked()), this, SLOT(updateEnabled()));
    connect(chkIndexRefresh, SIGNAL(clicked()), this, SLOT(updateEnabled()));
    connect(chkWorktreeRefresh, SIGNAL(clicked()), this, SLOT(updateEnabled()));
    connect(grpEnableAutoRefresh, SIGNAL(toggled(bool)), this, SLOT(updateEnabled()));
}

void RepoManConfigPage::apply()
{
    setModified(false);
}

void RepoManConfigPage::init()
{
    updateEnabled();
}

QByteArray RepoManConfigPage::pageId() const
{
    return "AutoRefresh";
}

QString RepoManConfigPage::pageName() const
{
    return trUtf8("Auto Refresh");
}

QByteArray RepoManConfigPage::groupId() const
{
    return "General";
}

QString RepoManConfigPage::groupName() const
{
    return trUtf8("Gerneral");
}

QWidget* RepoManConfigPage::widget()
{
    return this;
}

void RepoManConfigPage::updateEnabled()
{
    bool enabled = grpEnableAutoRefresh->isChecked();

    chkGitRefresh->setEnabled(enabled);
    chkIndexRefresh->setEnabled(enabled);
    chkWorktreeRefresh->setEnabled(enabled);

    spnGitRefresh->setEnabled(chkGitRefresh->isChecked() && enabled);
    spnIndexRefresh->setEnabled(chkIndexRefresh->isChecked() && enabled);
    spnWorktreeRefresh->setEnabled(chkWorktreeRefresh->isChecked() && enabled);

    setModified();
}
