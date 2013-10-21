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

#include "libMacGitverCore/Log/LogManager.hpp"
#include "libMacGitverCore/Log/LogTemplate.hpp"
#include "libMacGitverCore/Log/LogConsumer.hpp"

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
        Data* d = new Data;
        return d;
    }

    void Manager::addMessage(Type t, const QString& message)
    {
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

    //-- Manager::Data -------------------------------------------------------------------------- >8

    Manager::Data::Data()
    {
        nextId = 1;
        consumer = NULL;
    }

}
