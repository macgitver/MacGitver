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

#include "libLogger/Channel.hpp"

#include <QSharedData>

class QString;

namespace Log
{

    enum Type
    {
        Information,
        Normal,
        Warning,
        Error,
        Debug
    };

    class Template;
    class Event;
    class Consumer;

    class LOGGER_API Manager
    {
    public:
        static void release();

    public:
        void addMessage(const QString& message, Type t = Normal);
        void addEvent(Channel ch, Event event);

        void addTemplate(const Template& t);
        void addTemplate(Template&& t);
        Template findTemplate(const QString& name) const;

        void addChannel(const Channel& ch);
        Channel findChannel(const QString& name) const;

        const Channel::List& channels() const;

        void setLogConsumer(Consumer* consumer);
        Consumer* logConsumer() const;
    };

}
