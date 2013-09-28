
#include "RepoMan/Events.hpp"

namespace RM
{

    Events::Events()
    {
    }

    Events* Events::sSelf = NULL;

    Events* Events::self()
    {
        if (sSelf == NULL) {
            sSelf = new Events;
        }
        return sSelf;
    }

    void Events::addReceiver(EventsInterface* ev)
    {
        self()->mEvents.insert(ev);
    }

    void Events::delReceiver(EventsInterface* ev)
    {
        self()->mEvents.remove(ev);
    }

    void Events::repositoryOpened(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryOpened(repo);
        }
    }

    void Events::repositoryAboutToClose(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryAboutToClose(repo);
        }
    }

    void Events::repositoryActivated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryActivated(repo);
        }
    }

    void Events::repositoryDeactivated(Repo* repo)
    {
        foreach (EventsInterface* ei, mEvents) {
            ei->repositoryDeactivated(repo);
        }
    }

}
