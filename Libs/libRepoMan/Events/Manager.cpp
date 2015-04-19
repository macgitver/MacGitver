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

#include "libRepoMan/Events/Manager.hpp"

#include <QCoreApplication>
#include <QPointer>
#include <QDebug>
#include <QThread>

namespace RM
{

    namespace Internal
    {

        void ThreadMover::sendMovedEvent(const Event& e)
        {
            Q_ASSERT(QThread::currentThread() == thread());
            EventManager::self().sendEvent(e);
        }

    }

    EventManager::EventManager()
    {
        if(thread() != qApp->thread()) {
            qDebug() << "Moving EventManager to main thread";
            moveToThread(qApp->thread());
        }

        mMover = new Internal::ThreadMover(this);
    }

    void EventManager::sendEvent(const Event& ev)
    {
        if (QThread::currentThread() != thread()) {
            QMetaObject::invokeMethod(mMover, "sendMovedEvent", Qt::QueuedConnection,
                                      Q_ARG(RM::Event, ev));
            return;
        }
        emit repoEvent(ev);
    }

    EventManager& EventManager::self()
    {
        static QPointer<EventManager> s;
        if (!s) {
            s = new EventManager;
        }
        return *s;
    }

}

