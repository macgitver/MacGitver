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

#include "libRepoMan/Events/Event.hpp"
#include "libRepoMan/Events/EventData.hpp"

#include "libRepoMan/Frontend/Repo.hpp"

namespace RM
{


    Event::Event()
    {
    }

    Event::~Event()
    {
    }

    Event::Event(const Event& o)
        : d(o.d)
    {
    }

    Event::Event(Event&& o)
        : d(std::move(o.d))
    {
    }

    Event::Event(const std::shared_ptr<const Internal::EventData>& o)
        : d(o)
    {
    }

    Event::Event(std::shared_ptr<const Internal::EventData>&& o)
        : d(std::move(o))
    {
    }

    Event& Event::operator =(const Event& o)
    {
        d = o.d;
        return *this;
    }

    Event& Event::operator =(Event&& o)
    {
        d = std::move(o.d);
        return *this;
    }

    EventType Event::type() const
    {
        if (!d) {
            return EventType::None;
        }
        return d->type();
    }

    Frontend::Repo Event::repo() const
    {
        if (!d) {
            return Frontend::Repo();
        }
        return d->repo().lock();
    }

    Frontend::Base Event::baseContext() const
    {
        if (!d) {
            return Frontend::Base();
        }
        return d->context();
    }

}
