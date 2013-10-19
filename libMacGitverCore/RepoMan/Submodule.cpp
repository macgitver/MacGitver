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

#include "Submodule.hpp"

#include "RepoMan/Private/Dumper.hpp"
#include "RepoMan/Private/SubmodulePrivate.hpp"

namespace RM
{
    using namespace Internal;

    Submodule::Submodule(const Git::Repository& _repo, Repo *_parent)
        : Repo(_repo, *new SubmodulePrivate(this, _repo))
    {
        RM_D(Submodule);

        d->isSubModule = true;
        d->parent = static_cast<Repo*>(_parent);

        setDisplayAlias(_repo.name());

        if (!_repo.isBare()) {
            d->scanSubmodules();
        }

        d->linkToParent(_parent);
    }

    //-- SubmodulePrivate --------------------------------------------------------------------------

    SubmodulePrivate::SubmodulePrivate(Submodule* _pub, const Git::Repository& _repo)
        : RepoPrivate(_pub, _repo)
    {
    }

    ObjTypes SubmodulePrivate::objType() const
    {
        return SubmoduleObject;
    }

    void SubmodulePrivate::dumpSelf(Dumper& dumper) const
    {
        dumper.addLine(QString(QLatin1String("Submodule 0x%1"))
                       .arg(quintptr(mPub),0,16));
    }

    void SubmodulePrivate::postCreation()
    {
        BasePrivate::postCreation();
    }

    void SubmodulePrivate::preTerminate()
    {
        // Do we need to do smth?
        BasePrivate::preTerminate();
    }

    bool SubmodulePrivate::refreshSelf()
    {
        return true;
    }

    QString SubmodulePrivate::objectTypeName() const
    {
        return QLatin1String("Submodule");
    }

}
