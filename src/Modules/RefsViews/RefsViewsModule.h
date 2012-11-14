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

#ifndef MGV_MODULE_REFSVIEWS_H
#define MGV_MODULE_REFSVIEWS_H

#include "MacGitver/Module.h"

class RefsViewsModule : public Module
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.babbelbox.sacu.macgitver.Module/0.1" FILE "Module.json" )
    Q_INTERFACES( Module )

public:
    RefsViewsModule();

public:
    void setupConfigPages( IConfigDialog* dlg );

    void initialize();
    void deinitialize();

private:
    static Heaven::View* createBranchesView();
    static Heaven::View* createTagsView();
    static Heaven::View* createRefsView();
};

#endif
