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

#include "libLogger/LogManager.hpp"
#include "libLogger/LogTemplate.hpp"
#include "libLogger/LogConsumer.hpp"
#include "libLogger/LogEvent.hpp"
#include "libLogger/Internal.hpp"

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
        if (!channel) {
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
     * @brief       Add a template
     *
     * @param[in]   t       The template to add.
     *
     */
    void Manager::addTemplate(const Template& t)
    {
        Internal::System::self()->addTemplate(t);
    }

    /**
     * @brief       Add a template
     *
     * @param[in]   t       The template to add.
     *
     *
    void Manager::addTemplate(Template&& t)
    {
        Internal::System::self()->addTemplate(t);
    }*/

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
        return Internal::System::self()->findTemplate(name);
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
    void Manager::addChannel(const Channel& ch)
    {
        Internal::System::self()->addChannel(ch);
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
        return Internal::System::self()->findChannel(name);
    }

    /**
     * @brief       Get the list of channels
     *
     * @return      A Channel::List containing all registered channels.
     *
     */
    const Channel::List& Manager::channels() const
    {
        return Internal::System::self()->channels();
    }

    /**
     * @brief       Set the log consumer
     *
     * @param[in]   consumer    The new consumer to set.
     *
     * There can only be one consumer at a time. If one is already set, a warning will be output to
     * the debugging log.
     *
     * If @a consumer is `nullptr` the current consumer will be removed.
     *
     */
    void Manager::setLogConsumer(Consumer* consumer)
    {
        Internal::System::self()->setConsumer(consumer);
    }

    /**
     * @brief       Get the log consumer
     *
     * @return      The currently installed log consumer or `nullptr` if no consumer is installed.
     *
     */
    Consumer* Manager::logConsumer() const
    {
        return Internal::System::self()->consumer();
    }

    void Manager::release()
    {
        Internal::System::self()->release();
    }

}
