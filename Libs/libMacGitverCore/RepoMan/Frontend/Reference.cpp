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

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/RefName.hpp"

#include "RepoMan/Events.hpp"

#include "RepoMan/Frontend/BaseInternal.hpp"
#include "RepoMan/Frontend/Reference.hpp"
#include "RepoMan/Frontend/Repo.hpp"

#include "RepoMan/Data/Reference.hpp"

#include "RepoMan/Private/Dumper.hpp"

namespace RM
{

    namespace Frontend
    {

        #if 0

        Reference::Reference(Data::Reference* data)
            : Base(data)
        {
        }

        Reference::Reference(Base* parent, RefTypes type, const Git::Reference& ref)
            : Base( *new Data::Reference( this, type, ref ) )
        {
            RM_D(Reference);
            d->linkToParent( parent );
        }
        #endif

        RefTypes Reference::type() const
        {
            DPtrT<const Reference> d(this);
            return d->mType;
        }

        QString Reference::name() const
        {
            DPtrT<const Reference> d(this);
            return d->mName;
        }

        QString Reference::fullName() const
        {
            DPtrT<const Reference> d(this);
            return d->mFullQualifiedName;
        }

        Git::ObjectId Reference::id() const
        {
            DPtrT<const Reference> d(this);
            return d->mId;
        }

        QString Reference::displaySha1() const
        {
            return id().toString(8);
        }

        #if 0
        Git::Reference Reference::load(Git::Result& r)
        {
            RM_D(Reference);
            Git::Reference gitReference;

            if (r) {
                Git::Repository repo = repository()->gitRepo();
                gitReference = repo.Reference(r, d->mFullQualifiedName);
            }

            return gitReference;
        }

        #endif

    }

}
