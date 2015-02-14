/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef REPOMAN_BRANCH_PRIVATE_HPP
#define REPOMAN_BRANCH_PRIVATE_HPP

#include "RepoMan/Private/RefPrivate.hpp"

#include "RepoMan/Branch.hpp"

namespace RM
{

    namespace Internal
    {

        class BranchPrivate : public RefPrivate
        {
        public:
            BranchPrivate(Branch* pub, const Git::Reference& ref);

        public:
            ObjTypes objType() const;
            void postCreation();
            void preTerminate();
            bool refreshDetails(const Git::Reference& ref);
            void emitMoved();
            void dumpSelf(Dumper& dumper) const;
            QString objectTypeName() const;
            bool inherits(ObjTypes type) const;

        public:
            bool    mHasUpstream;
            int     mAheadCount;
            int     mBehindCount;
            QString mUpstreamRefName;
        };

    }

}

#endif
