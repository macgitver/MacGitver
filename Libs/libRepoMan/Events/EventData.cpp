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

#include "libRepoMan/Events/EventData.hpp"

namespace RM
{

    namespace Internal
    {

        EventData::EventData(EventType type, Data::Base* context)
            : mType(type)
            , mContext(context ? context->getPtr() : Data::Base::SPtr())
        {
        }

        std::shared_ptr<const EventData> EventData::create(EventType type, Data::Base* context)
        {
            return std::make_shared<const EventData>(type, context);
        }

        Data::Repo::WPtr EventData::repo() const
        {
            if (!mContext) {
                return Data::Repo::WPtr();
            }
            return mContext->repository();
        }

    }

}
