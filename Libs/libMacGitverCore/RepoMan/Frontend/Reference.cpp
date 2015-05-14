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

#include "RepoMan/Frontend/Reference.hpp"
#include "RepoMan/Repo.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Data/RefData.hpp"

namespace RM
{

    namespace Frontend
    {

        Reference::Reference(Internal::ReferencePrivate& data)
            : Base( data )
        {
        }

        Reference::Reference(Base* parent, ReferenceTypes type, const Git::Reference& Reference)
            : Base( *new Internal::ReferencePrivate( this, type, Reference ) )
        {
            RM_D( Reference );
            d->linkToParent( parent );
        }

        ReferenceTypes Reference::type() const
        {
            RM_CD(Reference);
            return d->mType;
        }

        QString Reference::name() const
        {
            RM_CD(Reference);
            return d->mName;
        }

        QString Reference::fullName() const
        {
            RM_CD(Reference);
            return d->mFullQualifiedName;
        }

        Git::ObjectId Reference::id() const
        {
            RM_CD(Reference);
            return d->mId;
        }

        QString Reference::displaySha1() const
        {
            return id().toString(8);
        }

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

    }

}
