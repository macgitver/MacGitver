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
#include "Dumper.hpp"

namespace RM
{

    RefLog::RefLog(Base* parent)
        : Base(parent)
    {
    }

    ObjTypes RefLog::objType() const
    {
        return RefLogObject;
    }

    void RefLog::dumpSelf(Internal::Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("RefLog 0x%1")).arg(quintptr(this),0,16));
    }

    void RefLog::preTerminate()
    {
        // What to do? We don't send Ref-Log-Deleted events
    }

}
