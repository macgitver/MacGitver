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

#include "libRepoMan/Core.hpp"

namespace RM
{

    namespace Internal
    {
        class Dumper;
    }

    namespace Data
    {
        class Base;
    }

    namespace Frontend
    {

        class REPOMAN_API Base
        {
        public:
            using List      = std::vector<Base>;
            using DPtrType  = Data::Base;

        public:
            Base(const std::shared_ptr<Data::Base>& _d);
            Base(std::shared_ptr<Data::Base>&& _d);
            virtual ~Base();
            Base();

        public:
            Base(const Base& other);
            Base(Base&& other);
            Base& operator=(const Base& other);
            Base& operator=(Base&& other);

            bool operator==(const Base& other) const;

            operator bool() const;

        public:
            ObjTypes objType() const;
            bool inherits(ObjTypes type) const;

        public:
            Repo repository() const;
            Base parent() const;

            Heaven::IconRef icon(bool small = false) const;

            QString typeName() const;
            QString displayName() const;
            QString dump() const;

            Heaven::Menu* contextMenu();

            std::shared_ptr<Data::Base> data() const;

            template<typename T>
            T as() const;

            List children() const;

        protected:
            std::shared_ptr<Data::Base> mData;

        protected:
            enum LockingMechanism { Locked, NotLocked };

        private:
            template<LockingMechanism LOCKED>
            struct Locker;

        protected:
            template<typename T, LockingMechanism LOCKED = Locked>
            class DPtrT;
        };

        template<typename T>
        inline T Base::as() const
        {
            using Dest = typename T::DPtrType;
            const Dest* p = std::static_pointer_cast<T, Data::Base>(mData);
            return T(p);
        }

    }

}
