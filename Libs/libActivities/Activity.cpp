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

#include "libActivities/Activity.hpp"
#include "libActivities/ActivityPrivate.hpp"
#include "libActivities/StepPrivate.hpp"
#include "libActivities/ManagerPrivate.hpp"

#include <cassert>

namespace Activities
{

    namespace
    {
        void countTypes(const std::vector<StepData::Ptr>& steps, int& pending, int& running,
                        int& success, int& error)
        {
            for (auto step : steps) {
                switch (step->mState) {
                case State::Unknown:
                case State::PartiallyFinishedWithErrors:
                    break;

                case State::Pending:
                    pending++;
                    break;

                case State::InProgress:
                    running++;
                    break;

                case State::Finished:
                    success++;
                    break;

                case State::FinishedWithErrors:
                    error++;
                    break;
                }
            }
        }

        State activityState(const std::vector<StepData::Ptr>& steps)
        {
            int pending = 0, running = 0, success = 0, error = 0, total = steps.size();
            countTypes(steps, pending, running, success, error);

            if (total == 0 || pending == total) {
                return State::Pending;
            }

            if (success == total) {
                return State::Finished;
            }

            if (error == total) {
                return State::FinishedWithErrors;
            }

            if (error > 0) {
                return State::PartiallyFinishedWithErrors;
            }

            assert(running + success > 0);
            return State::InProgress;
        }
    }

    ActivityData::ActivityData(const QString &display)
        : mGeneration(ManagerData::nextGeneration())
        , mState(State::Pending)
        , mIsDefunct(false)
        , mDisplay(display)
        , mCurProgress(0)
        , mMaxProgress(0)
        , mTotalCurProgress(0)
        , mTotalMaxProgress(0)
    {
    }

    ActivityData::~ActivityData()
    {
    }

    quint32 ActivityData::generation() const
    {
        return mGeneration;
    }

    ActivityData::Ptr ActivityData::getptr()
    {
        return shared_from_this();
    }

    std::unique_lock<std::mutex> ActivityData::uniqueLock() const
    {
        return std::unique_lock<std::mutex>(mMtx);
    }

    bool ActivityData::isDefunct() const
    {
        std::lock_guard<std::mutex> _(mMtx);
        return mIsDefunct;
    }

    void ActivityData::updated(const StepData::Ptr& step, std::unique_lock<std::mutex>& lock)
    {
        mTotalCurProgress = mCurProgress;
        mTotalMaxProgress = mMaxProgress;

        for (StepData::Ptr step : mSteps) {

        }
    }

    void ActivityData::stepStarted(const StepData::Ptr& step, std::unique_lock<std::mutex> &lock)
    {
        State newState = activityState(mSteps);
    }

    void ActivityData::stepFinished(const StepData::Ptr& step, bool failed, std::unique_lock<std::mutex> &lock)
    {
        State newState = activityState(mSteps);
    }

    void ActivityData::logUpdated(const LogData::Ptr &log, std::unique_lock<std::mutex> &lock)
    {

    }

    bool ActivityData::isModalityRequired() const
    {
        return mForceModalDialog;
    }

    // -----

    Step::Vector Activity::steps() const
    {
        Step::Vector r;
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            for (const StepData::Ptr& step : d->mSteps) {
                r.push_back(step);
            }
        }
        return r;
    }

    bool Activity::isDefunct() const
    {
        if (d) {
            return d->isDefunct();
        }

        return true;
    }

    Log Activity::log() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return {d->mLog};
        }
        return Log();
    }

    State Activity::state() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mState;
        }

        return State::Unknown;
    }

    QString Activity::display() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mDisplay;
        }

        return {};
    }

    int Activity::curProgress() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mTotalCurProgress;
        }

        return 0;
    }

    int Activity::maxProgress() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mTotalMaxProgress;
        }

        return 0;
    }

    int Activity::curOwnProgress() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mCurProgress;
        }

        return 0;
    }

    int Activity::maxOwnProgress() const
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);
            return d->mMaxProgress;
        }

        return 0;
    }

    Step Activity::createStep(const QString& displayName)
    {
        if (d) {
            std::lock_guard<std::mutex> _(d->mMtx);

            StepData::Ptr sd = StepData::create(d->getptr(), displayName);
            d->mSteps.push_back(sd);

            ManagerData::instance()->enqueue(EventTypes::StepAdded, d->getptr(), sd);

            return {sd};
        }

        return {};
    }

}
