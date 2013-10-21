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
#include <QHash>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libMacGitverCore/Log/LogManager.hpp"
#include "libMacGitverCore/Log/LogTemplate.hpp"
#include "libMacGitverCore/Log/LogConsumer.hpp"
#include "libMacGitverCore/Log/LogEvent.hpp"

#define CHNAME_DEBUG        QLatin1String("Debug")
#define CHNAME_NORMAL       QLatin1String("Normal")
#define CHNAME_ERROR        QLatin1String("Error")
#define CHNAME_INFO         QLatin1String("Info")
#define CHNAME_WARNING      QLatin1String("Warning")

namespace Log
{

    class Manager::Data : public QSharedData
    {
    public:
        Data();

    public:
        quint64 nextId;
        Consumer* consumer;
        QHash< QString, Template > templates;
        QHash< QString, Channel > channels;
    };

    Manager::Manager(const Manager& other)
        : d(other.d)
    {
    }

    Manager::Manager(Data* _d)
        : d(_d)
    {
    }

    Manager::Manager()
    {
    }

    Manager::~Manager()
    {
    }

    Manager& Manager::operator=(const Manager& other)
    {
        d = other.d;
        return *this;
    }

    bool Manager::isValid() const
    {
        return d;
    }

    Manager Manager::create()
    {
        Manager m(new Data);
        m.createDefaultChannels();
        return m;
    }

    void Manager::createDefaultChannels()
    {
        Channel ch = Channel::create(CHNAME_ERROR);
        ch.setDisplayName(MacGitver::trUtf8("Errors", "Channelname"));

        Template t = Template::create(CHNAME_ERROR);
        t.setTransformation(QLatin1String("<span style=\"color: red;\">$$</span>"));
        ch.setDefaultTemplate(t);
        addTemplate(t);

        addChannel(ch);

        ch = Channel::create(CHNAME_WARNING);
        ch.setDisplayName(MacGitver::trUtf8("Warnings", "Channelname"));

        t = Template::create(CHNAME_WARNING);
        t.setTransformation(QLatin1String("<span style=\"color: yellow;\">$$</span>"));
        addTemplate(t);
        ch.setDefaultTemplate(t);

        addChannel(ch);

        ch = Channel::create(CHNAME_INFO);
        ch.setDisplayName(MacGitver::trUtf8("Infos", "Channelname"));

        t = Template::create(CHNAME_INFO);
        t.setTransformation(QLatin1String("<span style=\"color: blue;\">$$</span>"));
        addTemplate(t);
        ch.setDefaultTemplate(t);

        addChannel(ch);

        ch = Channel::create(CHNAME_DEBUG);
        ch.setDisplayName(MacGitver::trUtf8("Debug", "Channelname"));

        t = Template::create(CHNAME_DEBUG);
        t.setTransformation(QLatin1String("<span style=\"color: navy;\">$$</span>"));
        addTemplate(t);
        ch.setDefaultTemplate(t);

        addChannel(ch);

        ch = Channel::create(CHNAME_NORMAL);
        ch.setDisplayName(MacGitver::trUtf8("Default output", "Channelname"));

        t = Template::create(CHNAME_NORMAL);
        t.setTransformation(QLatin1String("$$"));
        addTemplate(t);
        ch.setDefaultTemplate(t);

        addChannel(ch);
    }

    void Manager::addMessage(Type t, const QString& message)
    {
        QString channelName;
        switch(t) {
        case Error:         channelName = CHNAME_ERROR;     break;
        case Normal:        channelName = CHNAME_NORMAL;    break;
        case Information:   channelName = CHNAME_INFO;      break;
        case Warning:       channelName = CHNAME_WARNING;   break;
        case Debug:         channelName = CHNAME_DEBUG;     break;
        }

        Channel channel = findChannel(channelName);
        if (!channel.isValid()) {
            qDebug("Should have logged: %s - but got no valid channel.", qPrintable(message));
            return;
        }

        Event e = Event::create(channel.defaultTemplate(), message);
        channel.addEvent(e);
    }

    quint64 Manager::nextLogEventId()
    {
        return d ? d->nextId++ : 0;
    }

    void Manager::addTemplate(Template t)
    {
        Q_ASSERT(d && t.isValid());

        if (d && t.isValid()) {
            d->templates.insert(t.name(), t);
        }
    }

    Template Manager::findTemplate(const QString& name) const
    {
        Q_ASSERT(d);
        if (d) {
            return d->templates.value(name, Template());
        }

        return Template();
    }

    void Manager::addChannel(Channel ch)
    {
        Q_ASSERT(d);
        if (d) {
            d->channels.insert(ch.name(), ch);

            if (d->consumer) {
                d->consumer->channelAdded(ch);
            }
        }
    }

    Channel Manager::findChannel(const QString& name) const
    {
        Q_ASSERT(d);
        return d ? d->channels.value(name, Channel()) : Channel();
    }

    Channel::List Manager::channels() const
    {
        Q_ASSERT(d);
        Channel::List l;

        if (d) {
            foreach (Channel c, d->channels) {
                l << c;
            }
        }

        return l;
    }

    void Manager::setLogConsumer(Consumer* consumer)
    {
        Q_ASSERT(d);
        if (d) {
            if (consumer) {
                if (d->consumer != consumer) {
                    if (d->consumer) {
                        qDebug("A Log-Consumer was already set...");
                    }
                    d->consumer = consumer;
                }
            }
            else {
                d->consumer = NULL;
            }
        }
    }

    Consumer* Manager::logConsumer() const
    {
        return d ? d->consumer : NULL;
    }

    void Manager::eventAdded(Event event)
    {
        Q_ASSERT(d);
        if (d) {
            if (d->consumer) {
                d->consumer->eventAdded(event);
            }
        }
    }

    //-- Manager::Data -------------------------------------------------------------------------- >8

    Manager::Data::Data()
    {
        nextId = 1;
        consumer = NULL;
    }

}
