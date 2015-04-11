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

#include <QHash>
#include <QString>
#include <QSharedData>
#include <QMutex>

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

    class System
            : public QSharedData
    {
    public:
        using Ptr           = QExplicitlySharedDataPointer<System>;
        using Templates     = QHash<QString, Template>;
        using Channels      = QHash<QString, Channel>;
        using ChannelList   = QVector<Channel>;

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

        void addTemplate(const Template& t);
        Template findTemplate(const QString& name) const;

        void addChannel(const Channel& ch);
        Channel findChannel(const QString& name) const;
        ChannelList channels() const;

        void setConsumer(Consumer* consumer);
        Consumer* consumer() const;

    private:
        static Ptr      sSelf;
        mutable QMutex  mMtx;
        quint64         mNextId;
        Consumer*       mConsumer;
        Templates       mTemplates;
        Channels        mChannels;
    };

}
