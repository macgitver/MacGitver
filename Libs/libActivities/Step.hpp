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

#include "libActivities/Api.hpp"

#include <memory>
#include <vector>

class QString;

namespace Activities
{

    class Activity;
    class StepData;

    enum class State
    {
        Unknown,

        Pending,
        InProgress,
        Finished,
        FinishedWithErrors,

        PartiallyFinishedWithErrors
    };

    class ACTIVITIES_API Step
    {
    public:
        using Vector = std::vector<Step>;

    public:
        Step() = default;
        ~Step() = default;
        Step(const Step& o) : Step(o.d) {}
        Step(Step&& o) : Step(std::move(o.d)) {}
        Step(const std::shared_ptr<StepData>& o) : d(o) {}
        Step(std::shared_ptr<StepData>&& o) : d(std::move(o)) {}

    public:
        State state() const;
        QString displayName() const;
        QString statusText() const;
        int curProgress() const;
        int maxProgress() const;

    public:
        void setState(State newState);
        void setDisplayName(const QString& name);
        void setStatusText(const QString& text);
        void setProgress(int cur, int max);

    public:
        bool isDefunct() const;

    private:
        std::shared_ptr<StepData> d;
    };

}
