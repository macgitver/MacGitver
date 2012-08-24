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

#include "BackgroundTasks.h"

namespace Git
{

	BackgroundTask::BackgroundTask()
	{
		mThread = NULL;
	}

	bool BackgroundTask::run( BackgroundThead* bthread )
	{
		mThread = bthread;
		return execute();
	}

	BackgroundThead* BackgroundTask::worker()
	{
		return mThread;
	}

	BackgroundThead::BackgroundThead()
	{
	}

	void BackgroundThead::queue( BackgroundTask* task )
	{
		QMutexLocker locker( &mLock );

		task->moveToThread( this );
		mTasks.enqueue( task );
	}

	void BackgroundThead::run()
	{
		QMutexLocker locker( &mLock );
		bool cont = true;

		while( cont && !mTasks.isEmpty() )
		{
			BackgroundTask* task = mTasks.dequeue();
			locker.unlock();

			cont = task->run( this );
			task->deleteLater();

			locker.relock();
		}

		while( !mTasks.isEmpty() )
		{
			mTasks.dequeue()->deleteLater();
		}

		deleteLater();
	}

}
