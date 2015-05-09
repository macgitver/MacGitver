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

#pragma once

#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/Config/Ui/ConfigPage.hpp"
#include "libMacGitverCore/Config/Ui/ConfigPageProvider.hpp"

#include "ui_RepoManConfigPage.h"

namespace RM
{
    class AutoRefresher;
}

class RepoManConfigPage
        : public ConfigPage
        , private Ui::RepoManConfigPage
{
    Q_OBJECT
    DECLARE_NESTED_PAGE_CREATOR();

public:
    RepoManConfigPage(ConfigDialog* dlg);

public:
    void apply();
    void init();

private slots:
    void updateEnabled();

public:
    QByteArray pageId() const;
    QString pageName() const;
    QByteArray groupId() const;
    QString groupName() const;
    QWidget* widget();

private:
    RM::AutoRefresher* refresher;
};
