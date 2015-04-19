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

#include "libLogger/Event.hpp"
#include "libLogger/Channel.hpp"
#include "libLogger/Manager.hpp"
#include "libLogger/Template.hpp"
#include "libLogger/Consumer.hpp"

#include "libHeavenIcons/IconRef.hpp"

#include <QDateTime>
#include <QHash>
#include <QString>
#include <QMutex>
#include <QObject>

#include <memory>

#define CHNAME_DEBUG        QStringLiteral("Debug")
#define CHNAME_NORMAL       QStringLiteral("Normal")
#define CHNAME_ERROR        QStringLiteral("Error")
#define CHNAME_INFO         QStringLiteral("Info")
#define CHNAME_WARNING      QStringLiteral("Warning")

namespace Log
{

    class Template;
    class Channel;
    class Event;
    class Consumer;

    namespace Internal
    {

        class ThreadMover : public QObject
        {
            Q_OBJECT
        public slots:
            void newEvent(const Log::Event& e);
        };

        class System
                : public std::enable_shared_from_this<System>
        {
        public:
            using Ptr           = std::shared_ptr<System>;
            using Templates     = std::vector<Template>;
            using Channels      = std::vector<Channel>;

        public:
            System();
            ~System();

        public:
            void release();
            static Ptr self();

        public:
            quint64 nextLogEventId();
            void createDefaultChannels();
            void eventAdded(const Event& event);
            void emitEventAdded(const Event& event);

            void addTemplate(const Template& t);
            void addTemplate(Template&& t);
            Template findTemplate(const QString& name) const;

            void addChannel(const Channel& ch);
            Channel findChannel(const QString& name) const;
            const Channels& channels() const;

            void setConsumer(Consumer* consumer);
            Consumer* consumer() const;

        private:
            static Ptr      sSelf;
            mutable QMutex  mMtx;
            quint64         mNextId;
            Consumer*       mConsumer;
            Templates       mTemplates;
            Channels        mChannels;
            ThreadMover     mMover;
        };

        class ChannelData
                : public std::enable_shared_from_this<ChannelData>
        {
        public:
            ChannelData() {}

        public:
            Template        defaultTemplate;
            QString         name;
            QString         displayName;
            Heaven::IconRef icon;
            Event::List     events;
        };

        class TemplateData
                : public std::enable_shared_from_this<TemplateData>
        {
        public:
            TemplateData() {}

        public:
            QString         name;
            QString         transformation;
        };

        class EventData
                : public std::enable_shared_from_this<EventData>
        {
        public:
            EventData()
                : id(System::self()->nextLogEventId())
                , timeStamp(QDateTime::currentDateTime())
            {}

        public:
            std::weak_ptr<Channel::Data>    channel;
            Template                        htmlTemplate;
            quint64                         id;
            QDateTime                       timeStamp;
            QHash< QString, QString >       parameters;
        };

    }

}
