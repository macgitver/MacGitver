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

#include "libLogger/Api.hpp"

namespace Log
{

    class Channel;
    class Event;

    class LOGGER_API Consumer
    {
    public:
        Consumer();
        virtual ~Consumer();

    public:
        virtual void channelAdded(Channel channel) = 0;
        virtual void logCleaned(quint64 upToId) = 0;
        virtual void eventAdded(Event e) = 0;
    };

}
