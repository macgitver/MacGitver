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

#ifndef MGV_HISTORY_VIEW_H
#define MGV_HISTORY_VIEW_H

class QToolBar;

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/Config/ConfigUser.h"

#include "libHeaven/Views/GlobalView.hpp"
#include "libHeaven/Widgets/MiniSplitter.h"

#include "hic_HistoryViewActions.h"

class HistoryModel;
class HistoryDetails;
class HistoryList;
class HistoryListDelegate;

class HistoryDiff;

class HistoryView : public Heaven::GlobalView,
                    private HistoryViewActions,
                    private ConfigUser
{
    Q_OBJECT
public:
    HistoryView();

public slots:
    void repositoryChanged( Git::Repository repo );

private slots:
    void currentCommitChanged( const Git::ObjectId& sh1 );

private:
    void configChanged( const QString& subPath, const QVariant& value );

    void initSplitters();

private:
    Heaven::MiniSplitter*   mVertSplit;
    Heaven::MiniSplitter*   mHorzSplit;
    HistoryListDelegate*    mDelegate;
    HistoryModel*           mModel;
    HistoryList*            mList;
    HistoryDetails*         mDetails;
    HistoryDiff*            mDiff;
    Git::Repository         mRepo;
};

#endif

