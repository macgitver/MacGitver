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

#ifndef MGV_CORE_REPOMAN_REF_HPP
#define MGV_CORE_REPOMAN_REF_HPP

#include "libGitWrap/ObjectId.hpp"

#include "Base.hpp"

namespace RM
{

    enum RefTypes
    {
        BranchType,
        TagType,
        RootStageType
    };

    class Ref : public Base
    {
        Q_OBJECT
    public:
        Ref(Base* parent, RefTypes type, const QString& name);
        ~Ref();

    public:
        ObjTypes objType() const;

    public:
        RefTypes type() const;
        QString name() const;
        QString prefix() const;
        Git::ObjectId id() const;

    signals:
        void moved(Git::ObjectId fromId, Git::ObjectId toId);

    private:
        void read();

    protected:
        bool refreshSelf();

    private:
        RefTypes            mType;
        QString             mName;
        Git::ObjectId       mId;
    };

}

#endif
