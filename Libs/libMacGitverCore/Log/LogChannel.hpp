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
#ifndef MGV_LOG_LOG_CHANNEL_HPP
#define MGV_LOG_LOG_CHANNEL_HPP

#include <QSharedData>
#include <QVector>

class QString;

#include "libMacGitverCore/MacGitverApi.hpp"

namespace Heaven
{

    class IconRef;

}

namespace Log
{

    class Event;
    class Template;

    class MGV_CORE_API Channel
    {
    public:
        typedef QVector<Channel> List;

    public:
        class Data;
        Channel(Data* _d);
        Channel(const Channel& other);
        Channel();
        ~Channel();
        Channel& operator=(const Channel& other);
        bool isValid() const;

    public:
        static Channel create(const QString& name);

    public:
        void setDefaultTemplate(Template t);
        void setDisplayName(const QString& name);

    public:
        QString name() const;
        QString displayName() const;
        Heaven::IconRef icon() const;
        Template defaultTemplate() const;

        void addEvent(Event event);

    private:
        QExplicitlySharedDataPointer<Data> d;
    };

}

#endif
