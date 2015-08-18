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

<<<<<<< HEAD:Libs/libMacGitverCore/RepoMan/Frontend/Branch.hpp
#include "Ref.hpp"
=======
#include "libRepoMan/Frontend/Reference.hpp"
>>>>>>> 7799bdc... Split off RepoMan & make everything compile at least:Libs/libRepoMan/Frontend/Branch.hpp

namespace RM
{

    namespace Frontend
    {

        class REPOMAN_API Branch
                : public Reference
        {
        public:
            static const ObjTypes StaticObjectType = ObjTypes::Branch;
            typedef QVector< Branch* > List;

        public:
            Branch(Base* parent, const Git::Reference& ref);

        public:
            bool hasUpstream() const;
            QString upstreamRefName() const;
            Ref* upstream();
            int aheadCount() const;
            int behindCount() const;
            bool isHead() const;
        };

    }

}
