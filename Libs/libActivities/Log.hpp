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

#include <QStringList>

namespace Activities
{

    class LogData;

    class ACTIVITIES_API Log
    {
    public:
        Log() = default;
        ~Log() = default;
        Log(const Log& o) : Log(o.d) {}
        Log(Log&& o) : Log(std::move(o.d)) {}
        Log(const std::shared_ptr<LogData>& o) : d(o) {}
        Log(std::shared_ptr<LogData>&& o) : d(std::move(o)) {}

    public:
        void setRecoverable(const QString& text, bool flush = false);
        void addLine(const QString& text, bool flush = true);

    public:
        QStringList lines() const;

    public:
        bool isDefunct() const;

    private:
        std::shared_ptr<LogData> d;
    };

}
