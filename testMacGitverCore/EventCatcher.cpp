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

#include "libMacGitverCore/RepoMan/Repo.hpp"
#include "libMacGitverCore/RepoMan/Remote.hpp"
#include "libMacGitverCore/RepoMan/Ref.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "EventCatcher.hpp"

EventCatcher::EventCatcher()
{
    RM::Events::addReceiver(this);
}

EventCatcher::~EventCatcher()
{
    RM::Events::delReceiver(this);
}

void EventCatcher::clear()
{
    entries.clear();
}

EventCatcher::EventLogEntries EventCatcher::allEvents() const
{
    return entries;
}

int EventCatcher::eventCount() const
{
    return entries.count();
}

int EventCatcher::eventCount(EventTypes type)
{
    int count;

    foreach (const EventLogEntry& ele, entries) {
        if (ele.type == type) {
            count++;
        }
    }

    return count;
}

int EventCatcher::eventCount(EventTypes type, RM::Base* p1) const
{
    int count;

    foreach (const EventLogEntry& ele, entries) {
        if (ele.type == type && ele.params.count() == 1 && ele.params[0] == p1) {
            count++;
        }
    }

    return count;
}

int EventCatcher::eventCount(EventTypes type, RM::Base* p1, RM::Base* p2) const
{
    int count;

    foreach (const EventLogEntry& ele, entries) {
        if (ele.type == type && ele.params.count() == 2 && ele.params[0] == p1 &&
                ele.params[1] == p2) {
            count++;
        }
    }

    return count;
}

int EventCatcher::eventCount(EventTypes type, RM::Base* p1, RM::Base* p2, RM::Base* p3) const
{
    int count;

    foreach (const EventLogEntry& ele, entries) {
        if (ele.type == type && ele.params.count() == 3 && ele.params[0] == p1 &&
                ele.params[1] == p2 && ele.params[2] == p3) {
            count++;
        }
    }

    return count;
}


void EventCatcher::recordEvent(EventTypes type, RM::Base* p1)
{
    EventLogEntry ele;
    ele.type = type;
    ele.params << p1;
    entries << ele;
}

void EventCatcher::recordEvent(EventTypes type, RM::Base* p1, RM::Base* p2)
{
    EventLogEntry ele;
    ele.type = type;
    ele.params << p1 << p2;
    entries << ele;
}

void EventCatcher::recordEvent(EventTypes type, RM::Base* p1, RM::Base* p2, RM::Base* p3)
{
    EventLogEntry ele;
    ele.type = type;
    ele.params << p1 << p2 << p3;
    entries << ele;
}

void EventCatcher::repositoryOpened(RM::Repo* repo)
{
    recordEvent(ecRepoOpened, repo);
}

void EventCatcher::repositoryAboutToClose(RM::Repo* repo)
{
    recordEvent(ecRepoAboutToClose, repo);
}

void EventCatcher::repositoryActivated(RM::Repo* repo)
{
    recordEvent(ecRepoActivated, repo);
}

void EventCatcher::repositoryDeactivated(RM::Repo* repo)
{
    recordEvent(ecRepoDeactivated, repo);
}
