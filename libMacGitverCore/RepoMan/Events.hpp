/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#ifndef MGV_CORE_REPOMAN_EVENTS_HPP
#define MGV_CORE_REPOMAN_EVENTS_HPP

#include <QSet>

#include "libMacGitverCore/MacGitverApi.hpp"

namespace RM
{
    class Repo;

    class MGV_CORE_API EventsInterface
    {
    public:
        virtual ~EventsInterface() {}
    public:
        virtual void repositoryOpened(Repo* repo) = 0;
        virtual void repositoryAboutToClose(Repo* repo) = 0;
        virtual void repositoryActivated(Repo* repo) = 0;
        virtual void repositoryDeactivated(Repo* repo) = 0;
    };

    class MGV_CORE_API Events // : public EventsInterface
    {
    private:
        Events();

    public:
        static Events* self();
        static void addReceiver(EventsInterface* ev);
        static void delReceiver(EventsInterface* ev);

    private:
        QSet< EventsInterface* > mEvents;
        static Events* sSelf;

    public:
        void repositoryOpened(Repo* repo);
        void repositoryAboutToClose(Repo* repo);
        void repositoryActivated(Repo* repo);
        void repositoryDeactivated(Repo* repo);
    };

}

#endif
