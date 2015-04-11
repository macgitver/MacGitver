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

#include "libRepoMan/Frontend/Base.hpp"
#include "libRepoMan/Backend/RepoLocker.hpp"

namespace RM
{
    namespace Frontend
    {

        template<Base::LockingMechanism LOCKED>
        struct Base::Locker
        {
            /* constexpr */ Locker(const std::shared_ptr<Data::Repo>&)
            {}
        };

        template<>
        struct Base::Locker<Base::Locked>
                : private Backend::RepoLocker
        {
        public:
            Locker(const std::shared_ptr<const Data::Repo>& t)
                : Backend::RepoLocker(std::const_pointer_cast<Data::Repo>(t))
            {}
        };

        template<typename T, Base::LockingMechanism LOCKED>
        class Base::DPtrT
        {
        public:
            using DPtrType = typename T::DPtrType;

        public:
            DPtrT(T* that);

        public:
            const DPtrType* operator->() const      { return d.get(); }
            DPtrType* operator->()                  { return d.get(); }
            operator const DPtrType*() const        { return d.get(); }
            operator DPtrType*()                    { return d.get(); }

        private:
            std::shared_ptr<DPtrType> d;
            Base::Locker<LOCKED> l;
        };

        template<typename T, Base::LockingMechanism LOCKED>
        inline Base::DPtrT<T, LOCKED>::DPtrT(T* that)
            : d(std::static_pointer_cast<DPtrType, Data::Base>(that->mData))
            , l(d->repository().lock())
        {
        }

        template<typename T, Base::LockingMechanism LOCKED>
        class Base::DPtrT<const T, LOCKED>
        {
        public:
            using DPtrType = typename T::DPtrType;

        public:
            DPtrT(const T* that);

        public:
            const DPtrType* operator->() const      { return d.get(); }
            operator const DPtrType*() const        { return d.get(); }

        private:
            std::shared_ptr<const DPtrType> d;
            Base::Locker<LOCKED> l;
        };

        template<typename T, Base::LockingMechanism LOCKED>
        inline Base::DPtrT<const T, LOCKED>::DPtrT(const T* that)
            : d(std::static_pointer_cast<const DPtrType, const Data::Base>(that->mData))
            , l(d->repository().lock())
        {
        }

        template<typename T>
        inline typename T::List toFrontend(const typename T::DPtrType::SList& slist)
        {
            typename T::List frontendlist;
            for (const typename T::DPtrType::SPtr& s : slist) {
                frontendlist.push_back(T(s));
            }
            return frontendlist;
        }

        template<typename T>
        inline typename T::List toFrontend(const typename T::DPtrType::WList& wlist)
        {
            typename T::List frontendlist;
            for (const typename T::DPtrType::WPtr& w : wlist) {
                frontendlist.push_back(T(w.lock()));
            }
            return frontendlist;
        }

    }

}
