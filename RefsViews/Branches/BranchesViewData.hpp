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

#ifndef MGV_BRANCHES_VIEW_DATA_HPP
#define MGV_BRANCHES_VIEW_DATA_HPP

#include "libHeaven/CentralUI/Contexts/ViewContextData.hpp"

#include "libGitWrap/Repository.hpp"

class BranchesModel;

class BranchesViewData : public Heaven::ViewContextData
{
    Q_OBJECT
public:
    BranchesViewData();

private:
    void attachedToContext( Heaven::ViewContext* context );
    void detachedFromContext();

public:
    Git::Repository repository() const;

public:
    BranchesModel*  mModel;
};

#endif
