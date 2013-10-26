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

    /**
     * @class       Event
     * @brief       A single event in the logging system
     *
     * This class encapsulates a single event in the logging system. An event belongs to a Channel
     * and has an associated Template. Each event has a unique id and parameters.
     *
     * An event can be created in various ways. The most simple way is to call the log manager's
     * Log::Manager::addMessage() method. It will add an event consisting only of a message to one
     * of the default channels.
     *
     * Events can also be created by calling Event::create() or its overloads.
     *
     */

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

    /**
     * @brief       Copy constructor
     *
     * Creates a new reference to an existing Event.
     *
     * @param[in]   other       Event to create a copy of
     *
     */
    Event::Event(const Event& other)
        : d(other.d)
    {
    }

    /**
     * @internal
     * @brief       Creating constructor
     *
     * @param[in]   _d      Data to create the event with
     *
     */
    Event::Event(Data *_d)
        : d(_d)
    {
    }

    /**
     * @brief       Default constructor
     *
     * Creates an invalid Event object.
     */
    Event::Event()
    {
    }

    /**
     * @brief       Destructor
     *
     */
    Event::~Event()
    {
    }

    /**
     * @brief       Assignment operator
     *
     * @param[in]   other   Event to assign to this.
     *
     * @return      A reference to this.
     *
     */
    Event& Event::operator=(const Event& other)
    {
        d = other.d;
        return *this;
    }

    /**
     * @brief       Creates a simple new event
     *
     * The event is not assigned to a channel and can thus be still modified before the Consumer
     * is notified about the new event.
     *
     * @param[in]   tmpl    The template to assign to the event.
     *
     * @param[in]   text    A text to assign to the event. This text will be placed into the unnamed
     *                      parameter.
     *
     * @return      The new Event object.
     *
     */
    Event Event::create(Template tmpl, const QString& text)
    {
        Data* d = new Data;
        d->htmlTemplate = tmpl;
        d->parameters[QString()] = text;
        return d;
    }

    /**
     * @brief       Creates a new event
     *
     * The event is not assigned to a channel and can thus be still modified before the Consumer
     * is notified about the new event.
     *
     * @param[in]   tmpl    The template to assign to the event.
     *
     * @return      The new Event object.
     *
     */
    Event Event::create(Template tmpl)
    {
        Data* d = new Data;
        d->htmlTemplate = tmpl;
        return d;
    }

    /**
     * @brief       Creates a new event
     *
     * The event is not assigned to a channel and can thus be still modified before the Consumer
     * is notified about the new event.
     *
     * @param[in]   templ   The name of the template to assign to the event.
     *
     * @return      The new Event object. If the @a templ template cannot be looked up, an invalid
     *              Event object is returned.
     *
     */
    Event Event::create(const QString& templ)
    {
        Template t = MacGitver::log().findTemplate(templ);

        if (!t.isValid()) {
            return Event();
        }

        return create(t);
    }

    /**
     * @brief       Creates a simple text event
     *
     * The event is created with the @a text set as unnamed parameter and the @a channel's default
     * template as Template.
     *
     * The event will be added to the channel and the global consumer will immediately informed
     * about its existance. The event is not returned, as it is not possible to change the event
     * in a way that the global consumer would notice the change.
     *
     * @param[in]   channel     The channel to append the event to.
     *
     * @param[in]   text        The text to assign to the event.
     *
     */
    void Event::create(Channel channel, const QString& text)
    {
        channel.addEvent(create(channel.defaultTemplate(), text));
    }

    /**
     * @brief       Check for validity
     *
     * @return      `true` if this event object is valid, `false` otherwise.
     *
     */
    bool Event::isValid() const
    {
        return d;
    }

    /**
     * @internal
     * @brief       Set the channel
     *
     * @param[in]   channel     The channel to set for this event.
     *
     * This method internally sets a backward reference to the channel that owns the event. An
     * Event is owned by a Channel as soon as it is added to the channel.
     *
     */
    void Event::setChannel(Channel::Data* channel)
    {
        Q_ASSERT(d);
        if (d) {
            d->channel = channel;
        }
    }

    /**
     * @brief       Get the assigned channel
     *
     * @return      The channel that owns this Event, if any.
     */
    Channel Event::channel() const
    {
        return d->channel;
    }

    /**
     * @brief       Get the assigned template
     *
     * @return      The template that was assigned to this Event.
     *
     */
    Template Event::htmlTemplate() const
    {
        return d ? d->htmlTemplate : Template();
    }

    /**
     * @brief       Get the time stamp of this event
     *
     * @return      A QDateTime containing the date and time when this event object was created.
     *
     */
    QDateTime Event::timeStamp() const
    {
        return d ? d->timeStamp : QDateTime();
    }

    /**
     * @brief       Create HTML for this Event
     *
     * Applies the template to this event and gives the result.
     *
     * @return      HTML for this Event (as created by the Template)
     *
     */
    QString Event::html() const
    {
        if (!d) {
            return QString();
        }

        if (d->htmlTemplate.isValid()) {
            return d->htmlTemplate.apply(*this);
        }

        return QLatin1String("Missing template");
    }

    /**
     * @brief       Set a parameter
     *
     * @param[in]   param   Name of the parameter
     *
     * @param[in]   text    Text to set the parameter to
     *
     */
    void Event::setParam(const QString& param, const QString& text)
    {
        Q_ASSERT(d);

        if (d) {
            d->parameters[param] = text;
        }
    }

    /**
     * @brief       Get a parameter
     *
     * @param[in]   param   Name of the parameter to get
     *
     * @return      The parameter if it is found, an empty string otherwise.
     *
     */
    QString Event::param(const QString& param) const
    {
        if (d) {
            return d->parameters.value(param, QString());
        }

        return QString();
    }

    /**
     * @brief       Get a list of all parameter names
     *
     * @return      A QStringList with the names of all parameters
     *
     */
    QStringList Event::paramNames() const
    {
        return d ? d->parameters.keys() : QStringList();
    }

    /**
     * @brief       Get the unique id
     *
     * @return      This Event's unique identifier.
     *
     */
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
