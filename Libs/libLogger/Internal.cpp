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

#include "libLogger/Internal.hpp"

#include <QObject>
#include <QCoreApplication>
#include <QThread>

namespace Log
{

    namespace Internal
    {

        void ThreadMover::newEvent(const Log::Event& e)
        {
            System::self()->emitEventAdded(e);
        }

        System::System()
            : mNextId(1)
            , mConsumer(nullptr)
        {
            createDefaultChannels();
        }

        System::~System()
        {
        }


        void System::release()
        {
            sSelf.reset();
        }

        System::Ptr System::sSelf;

        System::Ptr System::self()
        {
            if (!sSelf) {
                sSelf = Ptr(new System);
            }
            return sSelf;
        }

        /**
         * @internal
         * @brief       create the default channels
         *
         * This method sets up the default channels that are used for convenience logging.
         *
         */
        void System::createDefaultChannels()
        {
            Channel ch = Channel::create(CHNAME_ERROR);
            ch.setDisplayName(QObject::trUtf8("Errors", "Channelname"));

            Template t = Template::create(CHNAME_ERROR);
            t.setTransformation(QStringLiteral("<span style=\"color: red;\">$$</span>"));
            ch.setDefaultTemplate(t);
            addTemplate(t);

            addChannel(ch);

            ch = Channel::create(CHNAME_WARNING);
            ch.setDisplayName(QObject::trUtf8("Warnings", "Channelname"));

            t = Template::create(CHNAME_WARNING);
            t.setTransformation(QStringLiteral("<span style=\"color: yellow;\">$$</span>"));
            addTemplate(t);
            ch.setDefaultTemplate(t);

            addChannel(ch);

            ch = Channel::create(CHNAME_INFO);
            ch.setDisplayName(QObject::trUtf8("Infos", "Channelname"));

            t = Template::create(CHNAME_INFO);
            t.setTransformation(QStringLiteral("<span style=\"color: blue;\">$$</span>"));
            addTemplate(t);
            ch.setDefaultTemplate(t);

            addChannel(ch);

            ch = Channel::create(CHNAME_DEBUG);
            ch.setDisplayName(QObject::trUtf8("Debug", "Channelname"));

            t = Template::create(CHNAME_DEBUG);
            t.setTransformation(QStringLiteral("<span style=\"color: navy;\">$$</span>"));
            addTemplate(t);
            ch.setDefaultTemplate(t);

            addChannel(ch);

            ch = Channel::create(CHNAME_NORMAL);
            ch.setDisplayName(QObject::trUtf8("Default output", "Channelname"));

            t = Template::create(CHNAME_NORMAL);
            t.setTransformation(QStringLiteral("$$"));
            addTemplate(t);
            ch.setDefaultTemplate(t);

            addChannel(ch);
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
        void System::eventAdded(const Event& event)
        {
            QMutexLocker l(&mMtx);

            if (mConsumer) {
                l.unlock();
                if (qApp->thread() == QThread::currentThread()) {
                    emitEventAdded(event);
                }
                else {
                    QMetaObject::invokeMethod(&mMover, "newEvent", Qt::QueuedConnection, Q_ARG(Event, event));
                }
            }
        }

        void System::emitEventAdded(const Event& event)
        {
            QMutexLocker l(&mMtx);

            if (Consumer* c = mConsumer) {
                l.unlock();
                c->eventAdded(event);
            }
        }

        void System::addTemplate(const Template& t)
        {
            QMutexLocker l(&mMtx);
            mTemplates.push_back(t);
        }

        void System::addTemplate(Template&& t)
        {
            QMutexLocker l(&mMtx);
            mTemplates.push_back(t);
        }

        Template System::findTemplate(const QString& name) const
        {
            QMutexLocker l(&mMtx);
            for (const Template& t : mTemplates) {
                if (t.name() == name) {
                    return t;
                }
            }
            return Template();
        }

        void System::addChannel(const Channel& ch)
        {
            QMutexLocker l(&mMtx);
            mChannels.push_back(ch);

            if (Consumer* c = mConsumer) {
                l.unlock();
                c->channelAdded(ch);
            }
        }

        Channel System::findChannel(const QString& name) const
        {
            QMutexLocker l(&mMtx);
            for (const Channel& ch : mChannels) {
                if (ch.name() == name) {
                    return ch;
                }
            }
            return Channel();
        }

        const System::Channels& System::channels() const
        {
            return mChannels;
        }

        void System::setConsumer(Consumer* consumer)
        {
            QMutexLocker l(&mMtx);

            if (consumer) {
                if (mConsumer != consumer) {
                    if (mConsumer) {
                        qDebug("A Log-Consumer was already set...");
                    }
                    mConsumer = consumer;
                }
            }
            else {
                mConsumer = nullptr;
            }
        }

        Consumer* System::consumer() const
        {
            QMutexLocker l(&mMtx);
            return mConsumer;
        }

        /**
         * @internal
         * @brief       Get tne next event id
         *
         * @return      The next available unique id for events.
         *
         */
        quint64 System::nextLogEventId()
        {
            QMutexLocker l(&mMtx);
            return mNextId++;
        }

    }

}
