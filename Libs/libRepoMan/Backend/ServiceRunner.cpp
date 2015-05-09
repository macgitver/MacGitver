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

#include "RepoMan/Backend/ServiceRunner.hpp"
#include "RepoMan/Backend/AbstractService.hpp"
#include "RepoMan/Backend/RepoMan.hpp"

#include <QQueue>
#include <QPointer>
#include <QMutex>
#include <QMutexLocker>

namespace RM
{

    namespace Backend
    {

        typedef QQueue< QPointer<AbstractService> > ServiceQueue;

        class ServiceRunner::Data
        {
        public:
            QMutex                  mMtx;

        public:
            ServiceQueue& queueFor(RunnerPriority priority);
            AbstractService* dequeueNext();

        private:
            ServiceQueue mLowQueue, mDefaultQueue, mUrgentQueue, mImmediateQueue;
        };

        ServiceQueue& ServiceRunner::Data::queueFor(RunnerPriority priority)
        {
            switch (priority) {
            case RunnerPriority::Low:       return mLowQueue;
            case RunnerPriority::Default:   return mDefaultQueue;
            case RunnerPriority::Urgent:    return mUrgentQueue;
            case RunnerPriority::Immediate: return mImmediateQueue;
            default:                        Q_ASSERT(false);
            }
        }

        AbstractService* ServiceRunner::Data::dequeueNext()
        {
            QMutexLocker _(&mMtx);

            if (!mImmediateQueue.empty()) {
                return mImmediateQueue.dequeue();
            }
            else if (!mUrgentQueue.empty()) {
                return mUrgentQueue.dequeue();
            }
            else if (!mDefaultQueue.empty()) {
                return mDefaultQueue.dequeue();
            }
            else if (!mLowQueue.empty()) {
                return mLowQueue.dequeue();
            }

            return NULL;
        }

        void ServiceRunner::processNext()
        {
            AbstractService* next = d->dequeueNext();

            if (next) {
                next->execute();
            }
        }

        ServiceRunner::ServiceRunner()
            : d(new Data)
        {
            connect(this, &ServiceRunner::processNextService,
                    this, &ServiceRunner::processNext,
                    Qt::QueuedConnection);
        }

        ServiceRunner::~ServiceRunner()
        {
            delete d;
        }

        void ServiceRunner::enqueue(Backend::AbstractService* svc, RunnerPriority priority)
        {
            QMutexLocker(&d->mMtx);

            ServiceQueue& queue(d->queueFor(priority));
            svc->moveToThread(Backend::RepoMan::workerThread());
            queue.enqueue(svc);

            emit processNextService();
        }

        ServiceRunner& ServiceRunner::instance()
        {
            QPointer< ServiceRunner > sSelf;

            if (sSelf.isNull()) {
                sSelf = new ServiceRunner;
                sSelf->moveToThread(Backend::RepoMan::workerThread());
            }

            return *sSelf;
        }

    }

}
