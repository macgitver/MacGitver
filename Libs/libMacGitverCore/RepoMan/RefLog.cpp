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

#include "RefLog.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/RefLogPrivate.hpp"

namespace RM
{

    using namespace Internal;

    RefLog::RefLog(Base* _parent)
        : Base(*new RefLogPrivate(this))
    {
        RM_D(RefLog);
        d->linkToParent(_parent);
    }

    //-- RefLogPrivate -----------------------------------------------------------------------------

    RefLogPrivate::RefLogPrivate(RefLog* _pub)
        : BasePrivate(_pub)
    {
    }

    ObjTypes RefLogPrivate::objType() const
    {
        return RefLogObject;
    }

    void RefLogPrivate::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("RefLog 0x%1"))
                       .arg(quintptr(mPub),0,16));
    }

    void RefLogPrivate::postCreation()
    {
        BasePrivate::postCreation();
    }

    void RefLogPrivate::preTerminate()
    {
        // What to do? We don't send Ref-Log-Deleted events
        BasePrivate::preTerminate();
    }

    bool RefLogPrivate::refreshSelf()
    {
        return true;
    }

    QString RefLogPrivate::objectTypeName() const
    {
        return QLatin1String("RefLog");
    }

    bool RefLogPrivate::inherits(ObjTypes type) const
    {
        return type == RefLogObject || BasePrivate::inherits(type);
    }

}
