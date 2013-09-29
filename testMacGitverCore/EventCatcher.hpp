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

#ifndef TEST_MGV_CORE_EVENT_CATCHER_HPP
#define TEST_MGV_CORE_EVENT_CATCHER_HPP

#include "libMacGitverCore/RepoMan/Base.hpp"
#include "libMacGitverCore/RepoMan/Events.hpp"

namespace RM
{
    class Repo;
}

enum EventTypes {
    ecRepoOpened,
    ecRepoAboutToClose,
    ecRepoActivated,
    ecRepoDeactivated
};

class EventCatcher : public RM::EventsInterface
{
public:
    struct EventLogEntry
    {
        EventTypes      type;
        RM::Base::List  params;
    };

    typedef QVector<EventLogEntry> EventLogEntries;

public:
    EventCatcher();
    ~EventCatcher();

public:
    void clear();
    EventLogEntries allEvents() const;

    int eventCount() const;
    int eventCount(EventTypes type);
    int eventCount(EventTypes type, RM::Base* p1) const;
    int eventCount(EventTypes type, RM::Base* p1, RM::Base* p2) const;
    int eventCount(EventTypes type, RM::Base* p1, RM::Base* p2, RM::Base* p3) const;

private:
    void recordEvent(EventTypes type, RM::Base* p1);
    void recordEvent(EventTypes type, RM::Base* p1, RM::Base* p2);
    void recordEvent(EventTypes type, RM::Base* p1, RM::Base* p2, RM::Base* p3);

protected:
    void repositoryOpened(RM::Repo* repo);
    void repositoryAboutToClose(RM::Repo* repo);
    void repositoryActivated(RM::Repo* repo);
    void repositoryDeactivated(RM::Repo* repo);

private:
    EventLogEntries entries;
};

#endif
