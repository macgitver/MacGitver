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

#include <QHash>
#include <QString>
#include <QDateTime>

#include "libMacGitverCore/Log/LogEvent.hpp"
#include "libMacGitverCore/Log/LogChannel.hpp"
#include "libMacGitverCore/Log/LogManager.hpp"
#include "libMacGitverCore/Log/LogTemplate.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

namespace Log
{

    class Event::Data : public QSharedData
    {
    public:
        Data();

    public:
        Channel::Data*              channel;
        Template                    htmlTemplate;
        quint64                     id;
        QDateTime                   timeStamp;
        QHash< QString, QString >   parameters;
    };

    Event::Event(const Event& other)
        : d(other.d)
    {
    }

    Event::Event(Data *_d)
        : d(_d)
    {
    }

    Event::Event()
    {
    }

    Event::~Event()
    {
    }

    Event& Event::operator=(const Event& other)
    {
        d = other.d;
        return *this;
    }

    Event Event::create(Template tmpl, const QString& text)
    {
        Data* d = new Data;
        d->htmlTemplate = tmpl;
        d->parameters[QString()] = text;
        return d;
    }

    Event Event::create(Template tmpl)
    {
        Data* d = new Data;
        d->htmlTemplate = tmpl;
        return d;
    }

    bool Event::isValid() const
    {
        return d;
    }

    void Event::setChannel(Channel::Data* channel)
    {
        Q_ASSERT(d);
        if (d) {
            d->channel = channel;
        }
    }

    Channel Event::channel() const
    {
        return d->channel;
    }

    Template Event::htmlTemplate() const
    {
        return d ? d->htmlTemplate : Template();
    }

    QDateTime Event::timeStamp() const
    {
        return d ? d->timeStamp : QDateTime();
    }

    QString Event::html() const
    {
        return QString();
    }

    void Event::setParam(const QString& param, const QString& text)
    {
        Q_ASSERT(d);

        if (d) {
            d->parameters[param] = text;
        }
    }

    QString Event::param(const QString& param) const
    {
        if (d) {
            return d->parameters.value(param, QString());
        }

        return QString();
    }

    QStringList Event::paramNames() const
    {
        return d ? d->parameters.keys() : QStringList();
    }

    quint64 Event::uniqueId() const
    {
        return d ? d->id : 0;
    }

    //-- Event::Data ---------------------------------------------------------------------------- >8

    Event::Data::Data()
    {
        channel = NULL;
        timeStamp = QDateTime::currentDateTime();
        id = MacGitver::log().nextLogEventId();
    }

}
