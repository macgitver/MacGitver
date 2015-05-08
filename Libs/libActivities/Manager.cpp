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

#include "libActivities/ManagerPrivate.hpp"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>

namespace Activities
{

    constexpr int ALLOWED_EMPTY_TIMERS  = 8;    // 8 * 750ms = 6 seconds before we stop "timing"
    constexpr int TIMER_INTERVAL        = 750;
    constexpr int SHOW_DIALOG_AFTER     = 5000;
    constexpr int HIDE_DIALOG_AFTER     = 30000;

    bool EventInfo::operator==(const EventInfo& o) const
    {
        if (type != o.type) {
            return false;
        }

        switch (type) {
        case EventTypes::StepAdded:
        case EventTypes::StepRemoved:
        case EventTypes::StepUpdated:
            if (step != o.step) {
                return false;
            }
            // fall through
        case EventTypes::LogUpdated:
        case EventTypes::ActivityAdded:
        case EventTypes::ActivityRemoved:
        case EventTypes::ActivityUpdated:
            if (activity != o.activity) {
                return false;
            }
            // fall through
        case EventTypes::GlobalState:
            break;
        }

        return true;
    }

    std::unique_lock<std::mutex> ManagerData::getLock()
    {
        static std::mutex sMutex;
        return std::unique_lock<std::mutex>(sMutex);
    }

    ManagerData::ManagerData()
        : mOwner(nullptr)
        , mUpdateTimer(nullptr)
        , mAllowedEmptyTimers(ALLOWED_EMPTY_TIMERS)
    {
        if (QThread::currentThread() != qApp->thread()) {
            // However we got created, force ourselves into the main thread...
            moveToThread(qApp->thread());
        }

        setParent(qApp);

        qRegisterMetaType<Activities::GlobalSnapShot>();
    }

    ManagerData::~ManagerData()
    {
        std::unique_lock<std::mutex> l(getLock());
        sSelf = nullptr;
    }

    ManagerData* ManagerData::sSelf = nullptr;

    quint32 ManagerData::nextGeneration()
    {
        static quint32 ng = 0;
        return ++ng;
    }

    ManagerData* ManagerData::instance()
    {
        if (!sSelf) {
            std::unique_lock<std::mutex> l(getLock());
            if (!sSelf) {
                sSelf = new ManagerData;
            }
        }

        return sSelf;
    }

    Activity ManagerData::newActivity(const QString& display)
    {
        ActivityData::Ptr ad { std::make_shared<ActivityData>(display) };

        mCurActivities.push_back(ad);
        enqueue(EventTypes::ActivityAdded, ad);

        return {ad};
    }

    void ManagerData::enqueue(EventInfo&& ei)
    {
        std::unique_lock<std::mutex> lock { getLock() };

        for (const EventInfo& temp : mEvents) {
            if (temp == ei) {
                return;
            }
        }

        mEvents.push_back(std::move(ei));

        if (maybeStartEventSending() || ei.type == EventTypes::GlobalState) {
            updateGlobalState();
        }
    }

    bool ManagerData::maybeStartEventSending()
    {
        if (mUpdateTimer && mUpdateTimer->isActive()) {
            return false;
        }

        if (!mUpdateTimer) {
            mUpdateTimer = new QTimer;

            if (QThread::currentThread() != thread()) { // Likely
                mUpdateTimer->moveToThread(thread());
            }

            mUpdateTimer->setParent(this);

            qDebug() << "Created update timer";

            connect(mUpdateTimer, &QTimer::timeout, this, &ManagerData::sendEvents);
        }

        mAllowedEmptyTimers = ALLOWED_EMPTY_TIMERS;
        mUpdateTimer->start(TIMER_INTERVAL);

        qDebug() << "Started the update timer";

        return true;
    }

    void ManagerData::sendEvents()
    {
        EventInfo::Vector events;

        if (QThread::currentThread() != qApp->thread()) {
            // This should actually not happen, since the timer ought to deliver events to the
            // right thread in the first place.
            qDebug() << "Activities::ManagerData::sendEvents() was called in non-gui-thread!";
            QMetaObject::invokeMethod(this, "sendEvents", Qt::QueuedConnection);
            return;
        }

        {
            // We're keeping the mutex lock as small as possible, so we don't bother the worker
            // thread to wait for us to send out events...
            std::unique_lock<std::mutex> lock { getLock() };
            events.swap(mEvents);

            if (events.empty()) {
                if (!--mAllowedEmptyTimers) {
                    qDebug() << "No events for " << ALLOWED_EMPTY_TIMERS * TIMER_INTERVAL << "ms. "
                                "Stopping the update timer.";
                    mUpdateTimer->stop();
                }
                return;
            }
            else {
                mAllowedEmptyTimers = ALLOWED_EMPTY_TIMERS;
            }
        }


        std::sort(events.begin(), events.end(), [](const EventInfo& a, const EventInfo& b) -> bool {

            if (a.type < b.type) {
                return true;
            }

            if (b.type < a.type) {
                return false;
            }

            assert(a.activity && b.activity);

            if (a.activity->generation() < b.activity->generation()) {
                return true;
            }
            if (b.activity->generation() < a.activity->generation()) {
                return false;
            }

            if (a.type == EventTypes::StepAdded ||
                    a.type == EventTypes::StepRemoved ||
                    a.type == EventTypes::StepUpdated) {

                assert(a.step && b.step);

                if (a.step->generation() < b.step->generation()) {
                    return true;
                }
                if (b.step->generation() < a.step->generation()) {
                    return false;
                }
            }

            // If we're still here, then we're the same type and neither one of the activity nor
            // step generation number is either less or greater -> We're equal! But we want to be
            // unique, so equality is impossible...
            assert(false);
            Q_UNREACHABLE();
        });

        for (EventInfo& ei : events) {
            switch (ei.type) {
            case EventTypes::ActivityAdded:
                mOwner->activityCreated({ei.activity});
                break;

            case EventTypes::ActivityRemoved:
                mOwner->activityRemoved({ei.activity});
                break;

            case EventTypes::ActivityUpdated:
                mOwner->activityUpdated({ei.activity});
                break;

            case EventTypes::LogUpdated:
                mOwner->logUpdated({ei.activity});
                break;

            case EventTypes::StepAdded:
                mOwner->stepCreated({ei.activity}, {ei.step});
                break;

            case EventTypes::StepRemoved:
                mOwner->stepRemoved({ei.activity}, {ei.step});
                break;

            case EventTypes::StepUpdated:
                mOwner->stepUpdated({ei.activity}, {ei.step});
                break;

            case EventTypes::GlobalState:
                {
                    std::unique_lock<std::mutex> lock(getLock());
                    mOwner->updateGlobalActivity(mCurrentGlobalState);
                }
                break;
            }
        }
    }

    bool ManagerData::isModalityRequired() const
    {
        bool required = false;

        for (const ActivityData::Ptr& a : mCurActivities) {
            if (a->isModalityRequired()) {
                required = true;
                break;
            }
        }

        return required;
    }

    void ManagerData::updateGlobalState()
    {

    }

    // -----

    Manager::Manager()
    {
        if (QThread::currentThread() != qApp->thread()) {
            // However we got created, force ourselves into the main thread...
            moveToThread(qApp->thread());
        }

        setParent(qApp);
    }

    Manager& Manager::instance()
    {
        ManagerData* d = ManagerData::instance();
        Q_ASSERT(d);
        if (!d->mOwner) {
            d->mOwner = new Manager;
        }
        return *d->mOwner;
    }

    Activity Manager::createActivity(const QString& display)
    {
        ManagerData* d = ManagerData::instance();
        std::unique_lock<std::mutex> lock(ManagerData::getLock());
        return d->newActivity(display);
    }

    Activity::Vector Manager::activities() const
    {
        ManagerData* d = ManagerData::instance();
        std::unique_lock<std::mutex> lock(ManagerData::getLock());

        Activity::Vector result;
        for (auto a : d->mCurActivities) {
            result.push_back({a});
        }

        return result;
    }

}
