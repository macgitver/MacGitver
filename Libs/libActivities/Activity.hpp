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
#include "libActivities/Log.hpp"

namespace Activities
{

    class ActivityData;

    class ACTIVITIES_API Activity
    {
    public:
        using Vector = std::vector<Activity>;

    public:
        Activity() = default;
        ~Activity() = default;
        Activity(const Activity& o) : Activity(o.d) {}
        Activity(Activity&& o) : Activity(std::move(o.d)) {}
        Activity(const std::shared_ptr<ActivityData>& o) : d(o) {}
        Activity(std::shared_ptr<ActivityData>&& o) : d(std::move(o)) {}

    public:
        Step::Vector steps() const;
        Step createStep(const QString& displayName);

    public:
        State state() const;
        QString display() const;
        int curProgress() const;
        int maxProgress() const;
        int curOwnProgress() const;
        int maxOwnProgress() const;
        Log log() const;

    public:
        void setState(State newState);
        void setDisplay(const QString& display);
        void setProgress(int cur, int max);

    public:
        bool isDefunct() const;

    private:
        std::shared_ptr<ActivityData> d;
    };

}
