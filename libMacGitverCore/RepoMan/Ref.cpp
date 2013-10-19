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
#include "libGitWrap/RefName.hpp"

#include "Ref.hpp"
#include "Repo.hpp"
#include "Events.hpp"

#include "Private/Dumper.hpp"
#include "Private/RefPrivate.hpp"

namespace RM
{

    using namespace Internal;

    Ref::Ref(RefPrivate& _data)
        : Base(_data)
    {
    }

    Ref::Ref(Base* _parent, RefTypes _type, const Git::Reference& _ref)
        : Base(*new RefPrivate(this, _type, _ref))
    {
        RM_D(Ref);
        d->linkToParent(_parent);
    }

    Ref::~Ref()
    {
    }

    RefTypes Ref::type() const
    {
        RM_D(Ref);
        return d->type;
    }

    QString Ref::name() const
    {
        RM_D(Ref);
        return d->name;
    }

    QString Ref::prefix() const
    {
        return QString();
    }

    Git::ObjectId Ref::id() const
    {
        RM_D(Ref);
        return d->id;
    }

    RefPrivate::RefPrivate(Ref* pub, RefTypes _type, const Git::Reference& _ref)
        : BasePrivate(pub)
        , type(_type)
    {
        fullQualifiedName = _ref.name();
        name = _ref.nameAnalyzer().name();
    }

    ObjTypes RefPrivate::objType() const
    {
        return RefObject;
    }

    QString RefPrivate::displayName() const
    {
        return name;
    }

    bool RefPrivate::refreshSelf()
    {
        Git::Result r;

        Repo* repo = repository();
        Git::Repository gr = repo->gitRepo();

        Git::Reference ref = gr.lookupRef(r, name);

        Git::ObjectId objectId = ref.objectId(r);

        if (objectId != id) {
            Events::self()->refMoved(repo, pub<Ref>());
            id = objectId;
        }

        return true;
    }

    void RefPrivate::postCreation()
    {
        RM_P(Ref);

        if (!repoEventsBlocked()) {
            Events::self()->refCreated(repository(), p);
        }

        BasePrivate::postCreation();
    }

    void RefPrivate::preTerminate()
    {
        RM_P(Ref);

        if (!repoEventsBlocked()) {
            Events::self()->refAboutToBeDeleted(repository(), p);
        }

        BasePrivate::preTerminate();
    }

    void RefPrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Ref 0x%1 [%2]"))
                       .arg(quintptr(mPub),0,16)
                       .arg(name));
    }

    QString RefPrivate::objectTypeName() const
    {
        return QLatin1String("Ref");
    }

}
