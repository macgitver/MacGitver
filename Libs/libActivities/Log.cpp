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

#include "libActivities/LogPrivate.hpp"
#include "libActivities/ActivityPrivate.hpp"

namespace Activities
{

    LogData::LogData()
    {
    }

    LogData::~LogData()
    {
    }

    LogData::Ptr LogData::getptr()
    {
        return shared_from_this();
    }

    LogData::Ptr LogData::create()
    {
        return std::make_shared<LogData>();
    }

    void LogData::setActivity(ActivityData* activity)
    {
        mActivity = activity->getptr();
    }

    void LogData::flush()
    {
        if (mRecoverableLine.isEmpty()) {
            return;
        }

        mLines << mRecoverableLine;
        mRecoverableLine = QString();
    }

    // ----

    void Log::setRecoverable(const QString& text, bool flush)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            if (flush) {
                d->flush();
            }
            d->mRecoverableLine = text;

            if (a) {
                a->logUpdated(d->getptr(), lock);
            }
        }
    }

    void Log::addLine(const QString& text, bool flush)
    {
        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            if (flush) {
                d->flush();
            }

            d->mLines << text;

            if (a) {
                a->logUpdated(d->getptr(), lock);
            }
        }
    }

    QStringList Log::lines() const
    {
        QStringList sl;

        if (d) {
            std::unique_lock<std::mutex> lock;
            ActivityData::Ptr a = d->mActivity.lock();
            if (a) {
                lock = a->uniqueLock();
            }

            sl = d->mLines;

            if (d->mRecoverableLine.isEmpty()) {
                // we definitely want to detach here, since we don't want to return a pointer into
                // the mutex-protected string list.
                sl.detach();
            }
            else {
                sl << d->mRecoverableLine;
            }
        }

        return sl;
    }

    bool Log::isDefunct() const
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
