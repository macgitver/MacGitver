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

#pragma once

#include "libBlueSky/Contexts.hpp"

#include "hic_RepoTreeViewCtxMenu.h"

class QModelIndex;

class RepoInfoModel;
class TreeViewCtxMenu;

#include "libMacGitverCore/RepoMan/Frontend/Repo.hpp"

class RepoTreeView
        : public BlueSky::ContextView
        , private RepoTreeViewCtxMenu
{
    Q_OBJECT
public:
    RepoTreeView();

private:
    void setupUi();

private slots:  // from actions
    void onCtxActivate();
    void onCtxClose();

private slots:  // from mRepos
    void contextMenu( const QModelIndex& index, const QPoint& globalPos );

private slots:  // for MacGitver::repoMan()
    void onRepoActivated(const RM::Frontend::Repo& repo);
    void onRepoDeactivated(const RM::Frontend::Repo& repo);

private:
    QModelIndex deeplyMapToSource( QModelIndex current ) const;
    BlueSky::ViewContext* createContextObject() const;

private:
    RepoInfoModel*          mModel;
    TreeViewCtxMenu*        mRepos;
};
