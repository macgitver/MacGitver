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

    //-- Ref --8>

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

    namespace Internal
    {

        RefPrivate::RefPrivate(Ref* pub, RefTypes type, const Git::Reference& ref)
            : BasePrivate( pub )
            , mType( type )
            , mFullQualifiedName( ref.name() )
            , mName( ref.nameAnalyzer().name() )
            , mId( ref.objectId() )
        {
        }

        void RefPrivate::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QLatin1String("Ref 0x%1 [%2]"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        ObjTypes RefPrivate::objType() const
        {
            return RefObject;
        }

        QString RefPrivate::displayName() const
        {
            return mName;
        }

        QString RefPrivate::objectTypeName() const
        {
            return QLatin1String("Ref");
        }

        bool RefPrivate::inherits(ObjTypes type) const
        {
            return type == RefObject || BasePrivate::inherits(type);
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

            if ( !repoEventsBlocked() ) {
                Events::self()->refAboutToBeDeleted(repository(), p);
            }

            BasePrivate::preTerminate();
        }

        void RefPrivate::emitMoved()
        {
            if (!repoEventsBlocked()) {
                Events::self()->refMoved(repository(), pub<Ref>());
            }
        }

        bool RefPrivate::refreshDetails(const Git::Reference& ref)
        {
            Git::ObjectId id = ref.objectId();

            if (id != mId) {
                mId = id;
                if (!repoEventsBlocked()) {
                    emitMoved();
                }
            }

            return true;
        }

        bool RefPrivate::refreshSelf()
        {
            Git::Result     r;
            Repo*           repo        = repository();          Q_ASSERT( repo );
            Git::Repository gr          = repo->gitLoadedRepo();
            Git::Reference  ref         = gr.reference(r, mFullQualifiedName);

            if (!ref.isValid() || ref.wasDestroyed()) {
                return false;
            }

            if (!refreshDetails(ref)) {
                return false;
            }

            return true;
        }

    }

}
