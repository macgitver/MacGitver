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

#include "libHeaven/CentralUI/Views/View.hpp"
#include "libHeaven/Widgets/MiniSplitter.hpp"

#include "WorkingTreeFilters.h"

#include "hic_WorkingTreeActions.h"

namespace DiffViews
{
    class DiffView;
}

class WorkingTreeModel;
class WorkingTreeItemView;

class QPlainTextEdit;


class IndexWidget : public Heaven::View, private WorkingTreeActions
{
    Q_OBJECT
public:
    IndexWidget();

    void setupFilters();

public slots:
    void repositoryChanged( Git::Repository repo );

private slots:
    void onShowAll( bool enabled );
    void onShowModified( bool enabled );
    void onShowMissing( bool enabled );
    void onShowIgnored( bool enabled );
    void onShowUntracked( bool enabled );
    void onShowUnchanged( bool enabled );
    void onShowStaged( bool enabled );
    //void workingTreeChanged();

private:
    void updateDiff();
    void setWtFilter(bool enabled, WorkingTreeFilter flag);

private:
    Heaven::MiniSplitter*   mSplitter;
    WorkingTreeItemView*    mTreeView;
    WorkingTreeModel*       mModel;
    DiffViews::DiffView*    mDiffView;
    WorkingTreeItemView*    mIndexTreeView;
    QPlainTextEdit*         mCommitMessage;

    WorkingTreeModel*       mStatusModel;

    Git::Repository         mRepo;
    bool                    mFilterRecursion;
};

#endif

