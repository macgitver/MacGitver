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

#include "libActivities/Activity.hpp"

#include <QObject>

namespace Activities
{

    struct GlobalSnapShot
    {
        int         mNumActivities;
        int         mRunningActivities;
        int         mDoneActivities;
        int         mFailedActivities;

        int         mCurProgress;
        int         mMaxProgress;

        QString     mUpfrontName;       // The name of the first registered activity. This is really
                                        // arbitrary as we obviously cannot display all of them in
                                        // the global snap shot.
    };

    class ACTIVITIES_API Manager
            : public QObject
    {
        Q_OBJECT
    private:
        Manager();
    public:
        ~Manager() = default;

        static Manager& instance();

    public:
        Activity createActivity(const QString& display);

    public:
        Activity::Vector activities() const;

    signals:
        void activityCreated(Activities::Activity);
        void activityUpdated(Activities::Activity);
        void activityRemoved(Activities::Activity);
        void stepCreated(Activities::Activity, Activities::Step);
        void stepUpdated(Activities::Activity, Activities::Step);
        void stepRemoved(Activities::Activity, Activities::Step);
        void logUpdated(Activities::Activity);

    signals:
        void updateGlobalActivity(Activities::GlobalSnapShot);
        void showProgressDialog(bool modal);
        void hideProgressDialog();
    };

}

Q_DECLARE_METATYPE(Activities::GlobalSnapShot)
