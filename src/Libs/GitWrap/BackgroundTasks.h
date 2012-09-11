/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef GIT_BACKGROUND_TASKS_H
#define GIT_BACKGROUND_TASKS_H

#include <QThread>
#include <QQueue>
#include <QMutex>

#include "Git.h"

namespace Git
{

	class BackgroundThead;

    /**
     * @brief The BackgroundTask class provides a common background worker thread.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API BackgroundTask : public QObject
	{
		Q_OBJECT
	public:
		BackgroundTask();

	public:
		bool run( BackgroundThead* bthread );

	protected:
		virtual bool execute() = 0;

		BackgroundThead* worker();

	private:
		BackgroundThead*		mThread;
	};

	class GITWRAP_API BackgroundThead : public QThread
	{
		Q_OBJECT
	public:
		BackgroundThead();

	public:
		void queue( BackgroundTask* task );

	protected:
		void run();

	private:
		QMutex						mLock;
		QQueue< BackgroundTask* >	mTasks;
	};

    /**@}*/
}

#endif
