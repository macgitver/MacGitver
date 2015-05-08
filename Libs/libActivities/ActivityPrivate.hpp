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

#include "libActivities/StepPrivate.hpp"
#include "libActivities/LogPrivate.hpp"

namespace Activities
{

    class ActivityData
            : public std::enable_shared_from_this<ActivityData>
    {
    public:
        using Steps     = std::vector<StepData::Ptr>;
        using Ptr       = std::shared_ptr<ActivityData>;
        using Vector    = std::vector<Ptr>;

    public:
        ActivityData(const QString& display);
        ~ActivityData();

    public:
        quint32 generation() const;
        Ptr getptr();
        std::unique_lock<std::mutex> uniqueLock() const;

        void updated(const StepData::Ptr& step, std::unique_lock<std::mutex> &lock);
        void stepStarted(const StepData::Ptr& step, std::unique_lock<std::mutex> &lock);
        void stepFinished(const StepData::Ptr& step, bool failed, std::unique_lock<std::mutex> &lock);
        void logUpdated(const LogData::Ptr& log, std::unique_lock<std::mutex>& lock);

    public:
        bool isModalityRequired() const;
        bool isDefunct() const;

    public:
        mutable std::mutex  mMtx;
        quint32             mGeneration;
        State               mState;
        bool                mIsDefunct;
        Steps               mSteps;
        LogData::Ptr        mLog;
        QString             mDisplay;
        int                 mCurProgress;
        int                 mMaxProgress;
        int                 mTotalCurProgress;
        int                 mTotalMaxProgress;
        bool                mForceModalDialog;
    };

}
