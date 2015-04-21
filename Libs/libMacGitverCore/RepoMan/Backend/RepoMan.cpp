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

#include "RepoMan/Backend/RepoMan.hpp"

#include <QThread>
#include <QPointer>

namespace RM
{

    namespace Backend
    {

        static RepoMan* repoMan()
        {
            static QPointer< RepoMan > sInstance;
            if (sInstance.isNull()) {
                sInstance = new RepoMan;
            }
            return sInstance;
        }

        RepoMan::RepoMan()
            : mWorkerThread(new QThread)
        {
            moveToThread(mWorkerThread);
            mWorkerThread->start();
        }

        RepoMan& RepoMan::instance()
        {
            return *repoMan();
        }

        QThread* RepoMan::workerThread()
        {
            return repoMan()->mWorkerThread;
        }

    }

}
