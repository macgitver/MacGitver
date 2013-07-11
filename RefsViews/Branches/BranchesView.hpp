/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef MGV_BRANCHES_VIEW_HPP
#define MGV_BRANCHES_VIEW_HPP

#include "libHeaven/CentralUI/Views/ContextView.hpp"

class QTreeView;
class QModelIndex;

#include "hic_BranchesViewActions.h"

namespace Git
{
    class Result;
}

class BranchesViewData;

class BranchesView : public Heaven::ContextView, private BranchesViewActions
{
    Q_OBJECT
public:
    BranchesView();

public:
    virtual QSize sizeHint() const;

public slots:
    void showContextMenu(const QModelIndex &index, const QPoint &globalPos);

    // hid actions
    void onCheckoutRef();
    void onRemoveRef();
    void onRenameRef();

    void actionFailed(const Git::Result &error);

protected:
    void contextMenuEvent(QContextMenuEvent *ev);

private:
    Heaven::ViewContextData* createContextData() const;
    virtual void attachedToContext( Heaven::ViewContext* ctx, Heaven::ViewContextData* data );
    virtual void detachedFromContext( Heaven::ViewContext* ctx );

private:
    QTreeView*          mTree;
    BranchesViewData*   mData;
};

#endif
