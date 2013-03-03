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

#include "libGitWrap/Repository.hpp"

class QTreeView;

#include "hic_BranchesViewActions.h"

class BranchesModel;

class BranchesData : public QObject
{
    Q_OBJECT
public:
    BranchesData( QObject* p ) : QObject( p ) {}

public:
    Git::Repository mRepo;
    BranchesModel*  mModel;
};

class BranchesView : public Heaven::View, private BranchesViewActions
{
    Q_OBJECT
public:
    BranchesView();

public slots:
    void repositoryChanged( Git::Repository repo );

public:
    virtual QSize sizeHint() const;

private slots:
    void rereadBranches();

private:
    QTreeView*      mTree;
    BranchesData*   mData;
};

#endif
