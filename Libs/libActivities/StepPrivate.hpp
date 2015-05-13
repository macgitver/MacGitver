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

#include "libActivities/Step.hpp"

#include <QString>
#include <QDebug>

#include <mutex>

namespace Activities
{

    class ActivityData;

    class StepData
            : public std::enable_shared_from_this<StepData>
    {
    public:
        using Ptr       = std::shared_ptr<StepData>;
        using Vector    = std::vector<Ptr>;

    public:
        StepData(const std::shared_ptr<ActivityData>& activity, const QString& display);
        ~StepData();

    public:
        static Ptr create(const std::shared_ptr<ActivityData>& activity, const QString& display);

    public:
        quint32 generation() const;
        Ptr getptr();

    public:
        QString displayName() const;
        QString statusText() const;
        int curProgress() const;
        int maxProgress() const;

    public:
        void changeState(State newState, std::unique_lock<std::mutex> &lock);
        void setDisplayName(const QString& name);
        void setStatusText(const QString& text);
        void setProgress(int cur, int max);

    public:
        std::weak_ptr<ActivityData> mActivity;
        quint32                     mGeneration;
        State                       mState;
        QString                     mStatusText;
        QString                     mDisplayName;
        int                         mCurProgress;
        int                         mMaxProgress;
    };

}

static inline
QDebug operator<<(QDebug dbg, Activities::State state)
{
    switch (state) {
    case Activities::State::Unknown:
        return dbg << "State::Unknown";
    case Activities::State::Pending:
        return dbg << "State::Pending";
    case Activities::State::Finished:
        return dbg << "State::Finished";
    case Activities::State::FinishedWithErrors:
        return dbg << "State::FinishedWithErrors";
    case Activities::State::InProgress:
        return dbg << "State::InProgress";
    case Activities::State::PartiallyFinishedWithErrors:
        return dbg << "State::PartiallyFinishedWithErrors";
    }
    Q_UNREACHABLE();
}
