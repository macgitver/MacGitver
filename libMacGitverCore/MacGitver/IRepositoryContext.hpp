
#ifndef MGVCORE_IREPOSITORY_CONTEXT_HPP
#define MGVCORE_IREPOSITORY_CONTEXT_HPP

#include <QObject>

class RepositoryInfo;

class IRepositoryContext
{
public:
    virtual RepositoryInfo* repository() = 0;
};

Q_DECLARE_INTERFACE( IRepositoryContext,
                     "org.macgitver.IRepositoryContext/0.1" )

#endif

