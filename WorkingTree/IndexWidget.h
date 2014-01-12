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

#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include <QWidget>

class QSplitter;

#include "libGitWrap/Repository.hpp"

#include "libBlueSky/Views.hpp"
#include "libBlueSky/MiniSplitter.hpp"

#include "hic_WorkingTreeActions.h"

namespace DiffViews
{
    class DiffView;
}

class WorkingTreeModel;
class WorkingTreeFilterModel;
class WorkingTreeItemView;
class IndexTreeItemView;

class QPlainTextEdit;

namespace RM
{
    class Repo;
}

class IndexWidget : public BlueSky::View, private WorkingTreeActions
{
    Q_OBJECT
public:
    IndexWidget();

    void setupFilters();

public slots:
    void repositoryActivated(RM::Repo* repo);
    void repositoryDeactivated(RM::Repo* repo);

private slots:
    void onShowAll();
    void onHideAll();
    void onShowModified( bool enabled );
    void onShowMissing( bool enabled );
    void onShowIgnored( bool enabled );
    void onShowUntracked( bool enabled );
    void onShowUnchanged( bool enabled );

    //void workingTreeChanged();

private:
    void updateDiff();

    void updateWtFilterView(const WorkingTreeFilterModel * const wtFilter);
    void setWtFilter(bool enabled, Git::Status flag);

private:
    BlueSky::MiniSplitter*  mSplitter;
    WorkingTreeItemView*    mTreeView;
    DiffViews::DiffView*    mDiffView;
    IndexTreeItemView*      mIndexTreeView;
    QPlainTextEdit*         mCommitMessage;

    WorkingTreeModel*       mStatusModel;

    RM::Repo*               mRepo;
    bool                    mFilterRecursion;
};

#endif

