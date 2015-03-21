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

#include "libMacGitverCore/RepoMan/Data/BaseData.hpp"

#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "libMacGitverCore/MacGitver/AutoRefresher.hpp"

#include "hic_RepoManActions.h"

namespace RM
{

    namespace Internal
    {

        class RepoManPrivate : public BasePrivate, private RepoManActions
        {
        public:
            RepoManPrivate(RepoMan* _pub);

        public:
            ObjTypes objType() const;
            bool refreshSelf();
            void preTerminate();
            QString displayName() const;
            void dumpSelf(Internal::Dumper& dumper) const;
            QString objectTypeName() const;

            Heaven::Menu* contextMenuFor(Base* object);

        public:
            Repo::List      repos;
            Repo*           activeRepo;
        };

    }

}
