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

    QString Ref::symbolicTarget() const
    {
        RM_CD( Ref );
        return d->mSymbolicTarget;
    }

    QString Ref::displaySha1() const
    {
        return id().toString(8);
    }


    namespace Internal
    {

        //-- Internal::RefPrivate --8>

        RefPrivate::RefPrivate(Ref* pub, RefTypes type, const Git::Reference& ref)
            : BasePrivate( pub )
            , mType( type )
            , mFullQualifiedName( ref.name() )
            , mName( ref.nameAnalyzer().name() )
            , mId( ref.objectId() )
            , mSymbolicTarget( ref.target() )
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

            Repo* repo = repository();
            if ( !repoEventsBlocked( repo ) ) {
                Events::self()->refCreated( repo, p );
            }

            BasePrivate::postCreation();
        }

        void RefPrivate::preTerminate()
        {
            RM_P(Ref);

            Repo* repo = repository();
            if ( !repoEventsBlocked( repo ) ) {
                Events::self()->refAboutToBeDeleted( repo, p );
            }

            BasePrivate::preTerminate();
        }

        bool RefPrivate::setId(const Git::ObjectId& id)
        {
            return ( id != mId ) ? mId = id, true : false;
        }

        bool RefPrivate::setSymbolicTarget(const QString &target)
        {
            return ( target != mSymbolicTarget ) ? mSymbolicTarget = target, true : false;
        }

        bool RefPrivate::refreshSelf()
        {
            Git::Result r;

            Repo* repo = repository();
            Q_ASSERT( repo );
            Git::Repository gr = repo->gitRepo();

            Git::Reference ref = gr.reference(r, mFullQualifiedName);
            if ( !ref.isValid() || ref.wasDestroyed() ) {
                return false;
            }

            bool moved = setId( ref.objectId() ) || setSymbolicTarget( ref.target() );
            if ( moved && !repoEventsBlocked( repo ) )
            {
                // send events after all values are set
                Events::self()->refMoved(repo, pub<Ref>());
            }

            return true;
        }

    }

}
