/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#pragma once

#include "libMacGitverCore/RepoMan/Data/Base.hpp"

#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "libMacGitverCore/MacGitver/AutoRefresher.hpp"

#include "hic_RepoManActions.h"

#if 0 // ###DEAD
namespace RM
{

    namespace Data
    {

        class RepoMan : private RepoManActions
        {
        public:
            RepoMan(RepoMan* _pub);

            Heaven::Menu* contextMenuFor(Base* object);

        public:
            Repo::List      repos;
            Repo*           activeRepo;
        };

    }

}
#endif
