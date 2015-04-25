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

namespace Heaven
{
    class IconRef;
}

namespace Log
{

    class Event;
    class Template;

    namespace Internal
    {
        class ChannelData;
    }

    class LOGGER_API Channel
    {
    public:
        using Data      = Internal::ChannelData;
        using List      = std::vector<Channel>;

    public:
        Channel(const std::shared_ptr<Data>& d);
        Channel(const Channel& other);
        Channel(Channel&& other);
        Channel();
        ~Channel();
        Channel& operator=(const Channel& other);
        Channel& operator=(Channel&& other);

    public:
        operator bool() const { return (bool) d; }

    public:
        static Channel create(const QString& name);

    public:
        void setDefaultTemplate(Template t);
        void setDisplayName(const QString& name);
        void setDisplayName(QString&& name);

    public:
        QString name() const;
        QString displayName() const;
        Heaven::IconRef icon() const;
        Template defaultTemplate() const;

        void addEvent(Event event);

    private:
        std::shared_ptr<Internal::ChannelData> d;
    };

}
