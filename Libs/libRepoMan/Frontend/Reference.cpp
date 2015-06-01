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

#include "libRepoMan/Frontend/BaseInternal.hpp"
#include "libRepoMan/Frontend/Reference.hpp"
#include "libRepoMan/Frontend/Repo.hpp"

#include "libRepoMan/Data/Reference.hpp"

#include "libRepoMan/Private/Dumper.hpp"

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
            : Base( *new Internal::RefPrivate( this, type, ref ) )
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
            RM_D(Ref);
            Git::Reference gitRef;

            if (r) {
                Git::Repository repo = repository()->gitRepo();
                gitRef = repo.reference(r, d->mFullQualifiedName);
            }

            return gitRef;
        }

        #endif

    }


    namespace Data
    {

        #if 0

        ReferencePrivate::ReferencePrivate(Reference* pub, RefTypes type, const Git::Reference& ref)
            : BasePrivate( pub )
            , mType( type )
            , mFullQualifiedName( ref.name() )
            , mName( ref.nameAnalyzer().name() )
            , mId( ref.objectId() )
        {
        }

        void ReferencePrivate::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Ref 0x%1 [%2]"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        ObjTypes ReferencePrivate::objType() const
        {
            return ObjTypes::Reference;
        }

        QString ReferencePrivate::displayName() const
        {
            return mName;
        }

        QString ReferencePrivate::objectTypeName() const
        {
            return QStringLiteral("Ref");
        }

        bool ReferencePrivate::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Reference || BasePrivate::inherits(type);
        }

        void ReferencePrivate::postCreation()
        {
            RM_P(Reference);

            if (!repoEventsBlocked()) {
                Events::self()->refCreated(repository(), p);
            }

            BasePrivate::postCreation();
        }

        void ReferencePrivate::preTerminate()
        {
            RM_P(Reference);

            if ( !repoEventsBlocked() ) {
                Events::self()->refAboutToBeDeleted(repository(), p);
            }

            BasePrivate::preTerminate();
        }

        void ReferencePrivate::emitMoved()
        {
            if (!repoEventsBlocked()) {
                Events::self()->refMoved(repository(), pub<Ref>());
            }
        }

        bool ReferencePrivate::refreshDetails(const Git::Reference& ref)
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

        bool ReferencePrivate::refreshSelf()
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

        #endif

    }

}
