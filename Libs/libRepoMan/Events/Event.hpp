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

#include <memory>

namespace RM
{

    namespace Internal
    {
        class EventData;
    }

    enum class EventType
    {
        None,

        RepoOpened,
        RepoAboutToClose,
        RepoActivated,
        RepoDeactivated,
    };

    class REPOMAN_API Event
    {
    public:
        Event();
        Event(std::shared_ptr<const Internal::EventData>&& d);
        Event(const std::shared_ptr<const Internal::EventData>& d);
        Event(const Event& o);
        Event(Event&& o);

        Event& operator=(const Event& o);
        Event& operator=(Event&& o);

        ~Event();

    public:
        EventType type() const;
        Frontend::Repo repo() const;

        Frontend::Base baseContext() const;

        template<typename T>
        T context() const
        {
            return baseContext().as<T>();
        }

    private:
        std::shared_ptr<const Internal::EventData> d;
    };

}
