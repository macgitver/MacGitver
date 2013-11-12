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

#ifndef MGV_HISTORY_VIEW_H
#define MGV_HISTORY_VIEW_H

class QToolBar;

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/Config/ConfigUser.h"

#include "libHeaven/CentralUI/Views/View.hpp"
#include "libHeaven/Widgets/MiniSplitter.hpp"

#include "hic_HistoryViewActions.h"

#include "HistoryModel.h"

namespace RM
{
    class Repo;
}

class HistoryDetails;
class HistoryList;
class HistoryListDelegate;

class HistoryDiff;

class HistoryView : public Heaven::View,
                    private HistoryViewActions,
                    private ConfigUser
{
    Q_OBJECT
public:
    HistoryView();

private slots:
    void repoActivated(RM::Repo* repo);
    void currentCommitChanged(const Git::ObjectId& sha1);

    void onChangeShowLocalBranches(bool checked);
    void onChangeShowAllBranches(bool checked);
    void onChangeShowHeadOnly(bool checked);
    void onChangeShowAllRefs(bool checked);

    void onChangeShowLocal(bool checked);
    void onChangeShowRemote(bool checked);
    void onChangeShowTags(bool checked);

private:
    void configChanged( const QString& subPath, const QVariant& value );

    void initSplitters();
    void setShowBranches(HistoryModel::Roots branches);

private:
    Heaven::MiniSplitter*   mVertSplit;
    Heaven::MiniSplitter*   mHorzSplit;
    HistoryListDelegate*    mDelegate;
    HistoryModel*           mModel;
    HistoryList*            mList;
    HistoryDetails*         mDetails;
    HistoryDiff*            mDiff;
    RM::Repo*               mRepo;
};

#endif

