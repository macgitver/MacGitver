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

#include "libRepoMan/Data/Reference.hpp"

#include "libRepoMan/Frontend/Repo.hpp"

#include "libRepoMan/Events.hpp"

#include "libRepoMan/Private/Dumper.hpp"

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/RefName.hpp"

namespace RM
{

    namespace Data
    {

        #if 0
        Reference::Reference(Ref* pub, RefTypes type, const Git::Reference& ref)
            : Base( pub )
            , mType( type )
            , mFullQualifiedName( ref.name() )
            , mName( ref.nameAnalyzer().name() )
            , mId( ref.objectId() )
        {
        }

        void Reference::dumpSelf(Internal::Dumper& dumper) const
        {
            dumper.addLine(QString(QStringLiteral("Ref 0x%1 [%2]"))
                           .arg(quintptr(mPub),0,16)
                           .arg(mName));
        }

        ObjTypes Reference::objType() const
        {
            return ObjTypes::Reference;
        }

        QString Reference::displayName() const
        {
            return mName;
        }

        QString Reference::objectTypeName() const
        {
            return QStringLiteral("Ref");
        }

        bool Reference::inherits(ObjTypes type) const
        {
            return type == ObjTypes::Reference || BasePrivate::inherits(type);
        }

        void Reference::postCreation()
        {
            RM_P(Ref);

            if (!repoEventsBlocked()) {
                Events::self()->refCreated(repository(), p);
            }

            BasePrivate::postCreation();
        }

        void Reference::preTerminate()
        {
            RM_P(Ref);

            if ( !repoEventsBlocked() ) {
                Events::self()->refAboutToBeDeleted(repository(), p);
            }

            BasePrivate::preTerminate();
        }

        void Reference::emitMoved()
        {
            if (!repoEventsBlocked()) {
                Events::self()->refMoved(repository(), pub<Ref>());
            }
        }

        bool Reference::refreshDetails(const Git::Reference& ref)
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

        bool Reference::refreshSelf()
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
