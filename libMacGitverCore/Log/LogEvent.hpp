/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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
#ifndef MGV_LOG_LOG_EVENT_HPP
#define MGV_LOG_LOG_EVENT_HPP

#include <QtGlobal>
#include <QSharedData>
#include <QVector>

class QString;
class QStringList;
class QDateTime;

#include "libMacGitverCore/Log/LogChannel.hpp"

namespace Log
{

    class Template;

    class MGV_CORE_API Event
    {
        friend class Channel;

    public:
        typedef QVector<Event> List;

    public:
        Event(const Event& other);
        Event();
        ~Event();
        Event& operator=(const Event& other);
        bool isValid() const;

    public:
        static Event create(Template tmpl, const QString& text);
        static Event create(const QString& templ);
        static Event create(Template tmpl);
        static void create(Channel channel, const QString& text);

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
        void setChannel(Channel::Data* d);

    private:
        class Data;
        Event(Data* _d);
        QExplicitlySharedDataPointer<Data> d;
    };

}

#endif
