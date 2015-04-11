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

#include <functional>
#include <vector>
#include <memory>

#ifdef _MSVC
#define const_or_constexpr const
#else
#define const_or_constexpr constexpr
#endif

namespace RM
{

    namespace Internal
    {
        class Dumper;
    }

    namespace Frontend
    {
        class Base;
    }

    namespace Data
    {

        class Repo;

        class Base
                : public std::enable_shared_from_this<Base>
        {
        public:
            using FrontendT = Frontend::Base;
            using SPtr      = std::shared_ptr<Base>;
            using WPtr      = std::weak_ptr<Base>;
            using SList     = std::vector<SPtr>;
            using WList     = std::vector<WPtr>;

        private:
            Base(const Base&) = delete;
            Base& operator=(const Base&) = delete;

        protected:
            Base();

        public:
            virtual ~Base();

        public:
            void dumpRecursive(Internal::Dumper& dumper) const;

            std::weak_ptr<Repo> repository() const;
            SPtr parent() const;

        public:
            virtual QString displayName() const;
            virtual QString objectTypeName() const = 0;
            virtual Heaven::IconRef icon(bool small) const;
            virtual bool inherits(ObjTypes type) const;
            virtual ObjTypes objType() const = 0;
            virtual void dumpSelf(Internal::Dumper& dumper) const = 0;
            void dumpChildren(Internal::Dumper& dumper) const;

            virtual WList children() const;

            template<typename T>
            std::shared_ptr<const T> as() const;

            template<typename T>
            std::shared_ptr<T> as();

        private:
            std::weak_ptr<Repo> mRepo;
            std::weak_ptr<Base> mParent;
        };

        template<typename T>
        inline std::shared_ptr<const T> Base::as() const
        {
            if (inherits(T::StaticObjectType)) {
                return std::shared_ptr<const T>(static_cast<const T*>(this));
            }
            return std::shared_ptr<const T>();
        }

        template<typename T>
        inline std::shared_ptr<T> Base::as()
        {
            if (inherits(T::StaticObjectType)) {
                return std::shared_ptr<T>(static_cast<T*>(this));
            }
            return std::shared_ptr<T>();
        }

        template<typename T>
        typename T::SList sharedFromWeakList(const typename T::WList& wlist)
        {
            typename T::SList slist;
            for (const typename T::WPtr& wptr : wlist) {
                if (auto sptr = wptr.lock()) {
                    slist.push_back(sptr);
                }
            }
            return slist;
        }

        template<typename T>
        typename T::WList weakFromSharedList(const typename T::SList& slist)
        {
            typename T::WList wlist;
            for (const typename T::SPtr& sptr : slist) {
                wlist.push_back(sptr);
            }
            return wlist;
        }

    }

}
