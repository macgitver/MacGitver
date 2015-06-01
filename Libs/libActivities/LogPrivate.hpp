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

#include "libActivities/Log.hpp"

#include <QString>

#include <mutex>

namespace Activities
{

    class ActivityData;

    class LogData
            : public std::enable_shared_from_this<LogData>
    {
    public:
        using Ptr       = std::shared_ptr<LogData>;

    public:
        LogData();
        ~LogData();

    public:
        static Ptr create();

    public:
        Ptr getptr();

    public:
        void setActivity(ActivityData* activity);

    public:
        void flush();

    public:
        std::weak_ptr<ActivityData> mActivity;
        QString                     mRecoverableLine;
        QStringList                 mLines;
    };

}
