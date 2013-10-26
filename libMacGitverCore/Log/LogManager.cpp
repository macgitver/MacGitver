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

    /**
     * @class       Manager
     * @brief       Manages the logging facilities
     *
     * The Log::Manager class is a central singleton, which is accessible via the static
     * MacGitver::log() method. Its purpose is to manage the logging facilities of MacGitver.
     *
     * In MacGitver, all log entries are categoriyed into channels. Each entry consists of a set of
     * strings (parameters) and is associated with a template. The template is responsible for
     * rendering the parameters into HTML, which is then viewed by the consumer.
     *
     */

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

    /**
     * @brief       Copy constructor
     *
     * @param[in]   other   Manager to create a copy of
     *
     * Actually, there exists only one Manager. This method is to keep it possible to use the
     * Manager as a value class.
     */
    Manager::Manager(const Manager& other)
        : d(other.d)
    {
    }

    /**
     * @internal
     * @brief       Constructor
     *
     * @param[in]   _d  The internally associated data.
     *
     * Creates a manager internally.
     *
     */
    Manager::Manager(Data* _d)
        : d(_d)
    {
    }

    /**
     * @brief       Default constructor
     *
     * Required, but should not be used. Creates an invalid Manager object.
     *
     */
    Manager::Manager()
    {
    }

    /**
     * @brief       Destructor
     *
     * Does nothing but release the internal data.
     *
     */
    Manager::~Manager()
    {
    }

    /**
     * @brief       Assignment operator
     *
     * @param[in]   other   The Manager object to assign from.
     *
     * @return      A reference to this.
     *
     * Just implemented to gain value class semantics.
     *
     */
    Manager& Manager::operator=(const Manager& other)
    {
        d = other.d;
        return *this;
    }

    /**
     * @brief       Test for validiy
     *
     * @return      `true`, if this is the global singleton and is valid; `false` otherwise.
     *
     */
    bool Manager::isValid() const
    {
        return d;
    }

    /**
     * @internal
     *
     * @brief       Creator function
     *
     * @return      Returns a new Manager
     *
     */
    Manager Manager::create()
    {
        Manager m(new Data);
        m.createDefaultChannels();
        return m;
    }

    /**
     * @internal
     * @brief       create the default channels
     *
     * This method sets up the default channels that are used for convenience logging.
     *
     */
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

    /**
     * @brief       Add a message to a default channel
     *
     * Adds a simple message event to one of the default channels. No special template is assigned.
     *
     * @param[in]   message The message to log.
     *
     * @param[in]   t       Type of the message. This is one of the Log::Type enumerations. It
     *                      defaults to Normal.
     */
    void Manager::addMessage(const QString& message, Type t)
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

        Event::create(channel, message);
    }

    /**
     * @brief       Add an event to a channel
     *
     * This is a convenience method to add an event to a channel.
     *
     * @param[in]   ch      The channel to add the event to.
     *
     * @param[in]   event   The event to add to the channel.
     *
     */
    void Manager::addEvent(Channel ch, Event event)
    {
        ch.addEvent(event);
    }

    /**
     * @internal
     * @brief       Get tne next event id
     *
     * @return      The next available unique id for events.
     *
     */
    quint64 Manager::nextLogEventId()
    {
        return d ? d->nextId++ : 0;
    }

    /**
     * @brief       Add a template
     *
     * @param[in]   t       The template to add.
     *
     */
    void Manager::addTemplate(Template t)
    {
        Q_ASSERT(d && t.isValid());

        if (d && t.isValid()) {
            d->templates.insert(t.name(), t);
        }
    }

    /**
     * @brief       Search for a template by its name
     *
     * @param[in]   name    The template name to look for.
     *
     * @return      Either a valid Template object if it was found or an invalid one if it could
     *              not be found.
     *
     */
    Template Manager::findTemplate(const QString& name) const
    {
        Q_ASSERT(d);
        if (d) {
            return d->templates.value(name, Template());
        }

        return Template();
    }

    /**
     * @brief       Add a logging channel
     *
     * @param[in]   ch      Adds a logging channel to the system.
     *
     * If the consumer is already registered, it will be informed about the new channel being
     * created.
     *
     */
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

    /**
     * @brief       Search for a channel by its name
     *
     * @param[in]   name    The channel name to look for
     *
     * @return      If found, a valid Channel object, otherwise a default constructed (invalid)
     *              one.
     *
     */
    Channel Manager::findChannel(const QString& name) const
    {
        Q_ASSERT(d);
        return d ? d->channels.value(name, Channel()) : Channel();
    }

    /**
     * @brief       Get the list of channels
     *
     * @return      A Channel::List containing all registered channels.
     *
     */
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

    /**
     * @brief       Set the log consumer
     *
     * @param[in]   consumer    The new consumer to set.
     *
     * There can only be one consumer at a time. If one is already set, a warning will be output to
     * the debugging log.
     *
     * If @a consumer is `NULL` the consumer will be removed.
     *
     */
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

    /**
     * @brief       Get the log consumer
     *
     * @return      The currently installed log consumer or `NULL` if no consumer is installed.
     *
     */
    Consumer* Manager::logConsumer() const
    {
        return d ? d->consumer : NULL;
    }

    /**
     * @internal
     * @brief       Inform log consumer of new event
     *
     * @param[in]   event   The new event that was added to a channel.
     *
     * This method is internally called from the Channel when a new Event was added to it. If a
     * Consumer is installed, it will be informed about the new event.
     *
     */
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
