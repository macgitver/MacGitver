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

#include "libActivities/StepPrivate.hpp"
#include "libActivities/ActivityPrivate.hpp"
#include "libActivities/ManagerPrivate.hpp"

namespace Activities
{

    StepData::StepData(const ActivityData::Ptr& activity, const QString& display)
        : mActivity(activity)
        , mGeneration(ManagerData::nextGeneration())
        , mState(State::Pending)
        , mDisplayName(display)
        , mCurProgress(0)
        , mMaxProgress(0)
    {
    }

    StepData::~StepData()
    {
    }

    StepData::Ptr StepData::getptr()
    {
        return shared_from_this();
    }

    quint32 StepData::generation() const
    {
        return mGeneration;
    }

    StepData::Ptr StepData::create(const ActivityData::Ptr& activity, const QString& display)
    {
        return std::make_shared<StepData>(activity, display);
    }

    void StepData::changeState(State newState, std::unique_lock<std::mutex>& lock)
    {
        ActivityData::Ptr a;

        if (mState == newState) {
            return;
        }

        if (newState == State::PartiallyFinishedWithErrors ||
                newState == State::Unknown) {
            qDebug() << "Invalid new state" << newState << "for Activities::Step";
            return;
        }

        switch (mState) {
        case State::Unknown:
        case State::PartiallyFinishedWithErrors:
            qDebug() << "Invalid current state" << newState << "for Activities::Step";
            return;

        case State::Pending:
            if (newState == State::InProgress) {
                mState = newState;
                if ((a = mActivity.lock())) {
                    a->stepStarted(getptr(), lock);
                }
            }
            // Fall through

        case State::InProgress:
            if (newState == State::Finished || newState == State::FinishedWithErrors) {
                mState = newState;
                if ((a = mActivity.lock())) {
                    a->stepFinished(getptr(), newState == State::FinishedWithErrors, lock);
                }
            }
            break;

        case State::Finished:
        case State::FinishedWithErrors:
            // No change allowed
            qDebug() << "Invalid Activities::Step state transition:"
                     << mState << "to" << newState;

            newState = mState;
            break;
        }
    }

    State Step::state() const
    {
        State s { State::Unknown };

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            s = d->mState;
        }

        return s;
    }

    QString Step::displayName() const
    {
        QString r;

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            r = d->mDisplayName;
        }
        return r;
    }

    QString Step::statusText() const
    {
        QString r;

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            r = d->mStatusText;
        }
        return r;
    }

    int Step::curProgress() const
    {
        int r = 0;

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            r = d->mCurProgress;
        }
        return r;
    }

    int Step::maxProgress() const
    {
        int r = 0;

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            r = d->mMaxProgress;
        }
        return r;
    }

    void Step::setState(State newState)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            d->changeState(newState, lock);
        }
    }

    void Step::setDisplayName(const QString& name)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            d->mDisplayName = name;

            if (a) {
                a->updated(d, lock);
            }
        }
    }

    void Step::setStatusText(const QString& text)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            d->mStatusText = text;

            if (a) {
                a->updated(d, lock);
            }
        }
    }

    void Step::setProgress(int cur, int max)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            d->mCurProgress = cur;
            d->mMaxProgress = max;

            if (a) {
                a->updated(d, lock);
            }
        }
    }

    bool Step::isDefunct() const
    {
        if (d) {
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                return a->isDefunct();
            }
        }

        return true;
    }

}
