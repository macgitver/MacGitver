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

#include <QString>

#include "libHeaven/Icons/IconRef.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libMacGitverCore/Log/LogChannel.hpp"
#include "libMacGitverCore/Log/LogEvent.hpp"
#include "libMacGitverCore/Log/LogManager.hpp"
#include "libMacGitverCore/Log/LogTemplate.hpp"

namespace Log
{

    class Channel::Data : public QSharedData
    {
    public:
        Data();

    public:
        Template        defaultTemplate;
        QString         name;
        QString         displayName;
        Heaven::IconRef icon;
        Event::List     events;
    };

    Channel::Channel(const Channel& other)
        : d(other.d)
    {
    }

    Channel::Channel()
    {
    }

    Channel::Channel(Channel::Data* _d)
        : d(_d)
    {
    }

    Channel Channel::create(const QString& name)
    {
        Data* d = new Data;
        d->name = name;
        return d;
    }

    Channel::~Channel()
    {
    }

    Channel& Channel::operator=(const Channel& other)
    {
        d = other.d;
        return *this;
    }

    bool Channel::isValid() const
    {
        return d;
    }

    QString Channel::name() const
    {
        return d ? d->name : QString();
    }

    Heaven::IconRef Channel::icon() const
    {
        return d ? d->icon : Heaven::IconRef();
    }

    void Channel::addEvent(Event event)
    {
        Q_ASSERT(d);
        if (d) {
            event.setChannel(d.data());
            d->events.append(event);

            MacGitver::log().eventAdded(event);
        }
    }

    void Channel::setDisplayName(const QString& name)
    {
        Q_ASSERT(d);
        if (d) {
            d->displayName = name;
        }
    }

    QString Channel::displayName() const
    {
        return d ? d->displayName : QString();
    }

    Template Channel::defaultTemplate() const
    {
        return d ? d->defaultTemplate : Template();
    }

    void Channel::setDefaultTemplate(Template t)
    {
        Q_ASSERT(d);
        if (d) {
            d->defaultTemplate = t;
        }
    }

    //-- Channel::Data -------------------------------------------------------------------------- >8

    Channel::Data::Data()
    {
    }

}
