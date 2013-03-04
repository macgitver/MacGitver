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

#include <QtPlugin>

#include "RefsViewsModule.h"
#include "TagsView.h"
#include "BranchesView.hpp"
#include "RefsView.h"

RefsViewsModule::RefsViewsModule()
{
}

Heaven::View* RefsViewsModule::createBranchesView()
{
    return new BranchesView();
}

Heaven::View* RefsViewsModule::createTagsView()
{
    return new TagsView();
}

Heaven::View* RefsViewsModule::createRefsView()
{
    return new RefsView();
}

void RefsViewsModule::initialize()
{
    registerView( "Branches",
                  tr( "Branches" ),
                  &RefsViewsModule::createBranchesView );

    registerView( "Refs",
                  tr( "Refs" ),
                  &RefsViewsModule::createRefsView );

    registerView( "Tags",
                  tr( "Tags" ),
                  &RefsViewsModule::createTagsView );
}

void RefsViewsModule::deinitialize()
{
    unregisterView( "Branches" );
    unregisterView( "Refs" );
    unregisterView( "Tags" );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( RefsViews, RefsViewsModule )
#endif
