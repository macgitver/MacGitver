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

#include "RepoMan/Ref.hpp"
#include "RepoMan/Repo.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Data/RefData.hpp"

namespace RM
{

    Ref::Ref(Internal::RefPrivate& data)
        : Base( data )
    {
    }

    Ref::Ref(Base* parent, RefTypes type, const Git::Reference& ref)
        : Base( *new Internal::RefPrivate( this, type, ref ) )
    {
        RM_D( Ref );
        d->linkToParent( parent );
    }

    RefTypes Ref::type() const
    {
        RM_CD(Ref);
        return d->mType;
    }

    QString Ref::name() const
    {
        RM_CD(Ref);
        return d->mName;
    }

    QString Ref::fullName() const
    {
        RM_CD(Ref);
        return d->mFullQualifiedName;
    }

    Git::ObjectId Ref::id() const
    {
        RM_CD(Ref);
        return d->mId;
    }

    QString Ref::displaySha1() const
    {
        return id().toString(8);
    }

    Git::Reference Ref::load(Git::Result& r)
    {
        RM_D(Ref);
        Git::Reference gitRef;

        if (r) {
            Git::Repository repo = repository()->gitRepo();
            gitRef = repo.reference(r, d->mFullQualifiedName);
        }

        return gitRef;
    }

}
