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

#include "hic_BranchesViewActions.h"

#include "libBlueSky/Contexts.hpp"

class QTreeView;
class QModelIndex;

namespace Git
{
    class Reference;
    class Result;
}

class BranchesViewData;

class BranchesView : public BlueSky::ContextView, private BranchesViewActions
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
    BlueSky::ViewContextData* createContextData() const;
    virtual void attachedToContext(BlueSky::ViewContext* ctx, BlueSky::ViewContextData* data );
    virtual void detachedFromContext(BlueSky::ViewContext* ctx );

    bool askToGoOn(const QString& message);
    inline bool checkRemoveRef(const Git::Reference &ref);

private:
    QTreeView*          mTree;
    BranchesViewData*   mData;
};

#endif
