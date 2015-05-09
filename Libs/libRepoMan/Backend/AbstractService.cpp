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

#include "libLogger/Manager.hpp"
#include "libLogger/Event.hpp"

#include "libRepoMan/Backend/AbstractService.hpp"
#include "libRepoMan/Backend/RepoMan.hpp"

#include <QDebug>

namespace RM
{

    namespace Backend
    {

        AbstractService::AbstractService()
        {
        }

        void AbstractService::execute()
        {
            if (thread() != RepoMan::workerThread()) {
                qDebug() << "Running a Service outside of the RepoMan Thread.";
                Q_ASSERT(false);
                return;
            }

            emit started();

            run();

            emit finished();
        }

        void AbstractService::logGitError(const Git::Result& r, const char* msg)
        {
            Log::Channel ch = RepoMan::instance().gitErrorChannel();
            Log::Template tpl = RepoMan::instance().gitErrorTemplate();
            Log::Event ev = Log::Event::create(tpl);

            ev.setParam(QStringLiteral("gwerror"), r.errorText());
            ev.setText(QString::fromUtf8(msg));

            Log::Manager().addEvent(ch, ev);
        }
    }

}
