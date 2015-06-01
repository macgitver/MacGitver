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

#include "libRepoMan/Events/Event.hpp"
#include "libRepoMan/Data/Base.hpp"
#include "libRepoMan/Data/Repo.hpp"

namespace RM
{

    namespace Internal
    {

        class EventData
        {
        public:
            EventData() = delete;
            EventData(const EventData&) = delete;
            EventData(EventType type, Data::Base* context);

        public:
            static std::shared_ptr<const EventData> create(EventType type, Data::Base* context);

        public:
            Data::Repo::WPtr repo() const;
            const Data::Base::SPtr& context() const { return mContext; }
            EventType type() const                  { return mType; }

        private:
            EventType           mType;
            Data::Base::SPtr    mContext;
        };


    }

}
