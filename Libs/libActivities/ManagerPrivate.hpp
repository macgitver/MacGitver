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

#include "libActivities/Manager.hpp"
#include "libActivities/ActivityPrivate.hpp"

class QTimer;

namespace Activities
{

    enum class EventTypes
    {
        ActivityAdded,
        ActivityUpdated,
        ActivityRemoved,

        StepAdded,
        StepUpdated,
        StepRemoved,

        LogUpdated,

        GlobalState
    };

    #if defined(__cpp_constexpr) && (__cpp_constexpr >= 201304L)
    constexpr
    #else
    static inline
    #endif
    int eventGroup(EventTypes et)
    {
        switch (et) {
        case EventTypes::ActivityAdded:     return 1;
        case EventTypes::StepAdded:         return 2;
        case EventTypes::ActivityUpdated:   return 3;
        case EventTypes::StepUpdated:       return 4;
        case EventTypes::LogUpdated:        return 5;
        case EventTypes::StepRemoved:       return 6;
        case EventTypes::ActivityRemoved:   return 7;
        case EventTypes::GlobalState:       return 8;
        }
    }

    #if defined(__cpp_constexpr) && (__cpp_constexpr >= 201304L)
    constexpr
    #else
    static inline
    #endif
    bool operator<(EventTypes a, EventTypes b)
    {
        int l1 = eventGroup(a), l2 = eventGroup(b);
        return l1 < l2;
    }

    struct EventInfo
    {
        using Vector        = std::vector<EventInfo>;

        EventTypes          type;
        ActivityData::Ptr   activity;
        StepData::Ptr       step;

        bool operator==(const EventInfo& o) const;
        EventInfo(EventTypes t, ActivityData::Ptr a, StepData::Ptr s = {})
            : type(t), activity(a), step(s)
        {}
    };

    class Manager;

    class ManagerData
            : public QObject
    {
        Q_OBJECT
    private:
        ManagerData();
    public:
        ~ManagerData();

    public:
        static ManagerData* instance();
        static std::unique_lock<std::mutex> getLock();
        static quint32 nextGeneration();

    public:
        bool isModalityRequired() const;
        Activity newActivity(const QString &display);

    public:
        void enqueue(EventInfo&& ei);

        template<typename ... T>
        void enqueue(T&& ... t)
        {
            EventInfo ei(std::forward<T>(t)...);
            enqueue(std::move(ei));
        }

    private slots:
        void sendEvents();

    private:
        bool maybeStartEventSending();
        void updateGlobalState();

    public:
        Manager*                mOwner;
        ActivityData::Vector    mCurActivities;
        QTimer*                 mUpdateTimer;
        int                     mAllowedEmptyTimers;
        EventInfo::Vector       mEvents;
        GlobalSnapShot          mCurrentGlobalState;

    private:
        static ManagerData* sSelf;
    };

}
