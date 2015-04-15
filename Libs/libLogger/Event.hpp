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

#include <memory>
#include <vector>

class QString;
class QStringList;
class QDateTime;

namespace Log
{

    class Template;
    class Channel;

    namespace Internal {
        class ChannelData;
        class EventData;
    }

    class LOGGER_API Event
    {
    public:
        using   List        = std::vector<Event>;

    public:
        Event(const Event& other);
        Event(Event&& other);
        Event();
        ~Event();
        Event& operator=(const Event& other);

        operator bool() const { return (bool) d; }

    public:
        static Event create(Template tmpl, const QString& text);
        static Event create(const QString& templ);
        static Event create(Template tmpl);
        static void create(Channel channel, const QString& text);

    public:
        void setChannel(const std::shared_ptr<Internal::ChannelData>&  d);

    public:
        Template htmlTemplate() const;
        Channel channel() const;

        quint64 uniqueId() const;

        QDateTime timeStamp() const;
        QString html() const;

        void setParam(const QString& param, const QString& text);
        QString param(const QString& param) const;

        QStringList paramNames() const;

    private:
        Event(const std::shared_ptr<Internal::EventData>& _d);
        std::shared_ptr<Internal::EventData> d;
    };

}

