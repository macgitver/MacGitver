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

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Reference.hpp"

#include "Ref.hpp"
#include "Repo.hpp"

namespace RM
{

    Ref::Ref(Base* parent, RefTypes type, const QString& name)
        : Base(parent)
        , mType(type)
        , mName(name)
    {
        read();
    }

    Ref::~Ref()
    {
    }

    ObjTypes Ref::objType() const
    {
        return RefObject;
    }

    RefTypes Ref::type() const
    {
        return mType;
    }

    QString Ref::name() const
    {
        return mName;
    }

    QString Ref::prefix() const
    {
        return QString();
    }

    Git::ObjectId Ref::id() const
    {
        return mId;
    }

    void Ref::read()
    {
        Git::Result r;

        Repo* repo = repository();
        Git::Repository gr = repo->gitRepo();

        Git::Reference ref = gr.lookupRef(r, mName);

        Git::ObjectId id = ref.objectId(r);

        if (id != mId) {
            emit moved(mId, id);
            mId = id;
        }
    }

}
