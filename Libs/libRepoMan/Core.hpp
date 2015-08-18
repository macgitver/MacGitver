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

#pragma once

#include <QVector>
#include <QSet>
#include <QStringList>
#include <QStringBuilder>

#include <qglobal.h>

#ifdef RepoMan_EXPORTS
#   define REPOMAN_API Q_DECL_EXPORT
#else
#   define REPOMAN_API Q_DECL_IMPORT
#endif

namespace Heaven {

    class Menu;
    class IconRef;

}

namespace RM
{

    enum class ObjTypes
    {
        Invalid,

        Namespace,
        Repo,
        Remote,
        Submodule,
        Head,
        Branch,
        Reference,
        RefTreeNode,
        Tag,
        RefLog
    };

    namespace Frontend
    {
        class Base;
        class Repo;
        class RefTreeNode;
        class Namespace;
        class Reference;
        class Remote;
        class RefLog;
        class Submodule;
        class Tag;
        class Branch;
    }

}
