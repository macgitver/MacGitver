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

#include "libGitWrap/GitWrap.hpp"

#include "MacGitver/MacGitver.h"
#include "MacGitver/Modules.h"
#include "MacGitver/FSWatcher.h"
#include "MacGitver/RepoManager.hpp"

#include "Interfaces/IMainWindow.h"

MacGitver::MacGitver( int argc, char** argv )
    : QApplication( argc, argv )
    , mMainWindow( NULL )
    , mLog( NULL )
    , mRepoMan( NULL )
{
    setOrganizationName( QLatin1String( "SaCu" ) );
    setApplicationName( QLatin1String( "MacGitver" ) );

    mWatcher = new FSWatcher( this );

    mModules = new Modules( this );

    mRepoMan = new RepoManager( this );

    Q_ASSERT( sSelf == NULL );
    sSelf = this;
}

MacGitver::~MacGitver()
{
    setRepository( Git::Repository() );

    delete mRepoMan;
    delete mModules;
    sSelf = NULL;
}

Modules* MacGitver::modules()
{
    return mModules;
}

FSWatcher* MacGitver::watcher()
{
    return mWatcher;
}

Git::Repository MacGitver::repository() const
{
    return mRepository;
}

void MacGitver::setRepository( const Git::Repository& repo )
{
    mRepository = repo;

    mWatcher->setRepository( repo );
    mModules->repositoryChanged( repo );

    emit repositoryChanged( mRepository );
}

IMainWindow* MacGitver::mainWindow()
{
    Q_ASSERT( mMainWindow );
    return mMainWindow;
}

void MacGitver::setMainWindow( IMainWindow* mainWindow )
{
    Q_ASSERT( !mMainWindow );
    mMainWindow = mainWindow;
}

MacGitver* MacGitver::sSelf = NULL;

MacGitver& MacGitver::self()
{
    Q_ASSERT( sSelf );
    return *sSelf;
}

void MacGitver::registerView( const QString& identifier, Heaven::ViewTypes type,
                              ViewCreator* creator )
{
    Q_ASSERT( !mViews.contains( identifier ) );

    ViewInfo vi;
    vi.mIdentifier = identifier;
    vi.mType = type;
    vi.mCreator = creator;
    mViews.insert( identifier, vi );
}

void MacGitver::unregisterView( const QString& identifier )
{
    mViews.remove( identifier );
}

Heaven::View* MacGitver::createView( const QString& identifier )
{
    if( mViews.contains( identifier ) )
    {
        ViewInfo vi = mViews.value( identifier );
        return vi.mCreator();
    }

    return NULL;
}

ILog* MacGitver::log()
{
    return mLog;
}

void MacGitver::setLog( ILog* log )
{
    mLog = log;
}

void MacGitver::log( LogType type, const QString& logMessage )
{
    if( mLog )
    {
        mLog->addMessage( type, logMessage );
    }
}

void MacGitver::log( LogType type, const char* logMessage )
{
    if( mLog )
    {
        mLog->addMessage( type, QString::fromUtf8( logMessage ) );
    }
}

void MacGitver::log( LogType type, const Git::Result& r, const char* logMessage )
{
    if( mLog )
    {
        if( logMessage )
        {
            mLog->addMessage( type, QString::fromUtf8( "GitWrap-Error: %1\n(%2)" )
                              .arg( r.errorText() ).arg( QLatin1String( logMessage ) ) );
        }
        else
        {
            mLog->addMessage( type, QString::fromUtf8( "GitWrap-Error: %1" )
                              .arg( r.errorText() ) );
        }
        }
}

RepoManager* MacGitver::repoMan()
{
    return mRepoMan;
}

// Because it's internal; this method will go as soon as it's user is also
// internal to libMacGitverCore
#include "Widgets/RepoStateWidget.hpp"

QWidget* MacGitver::createRepoStateWidget()
{
    return new RepoStateWidget;
}
