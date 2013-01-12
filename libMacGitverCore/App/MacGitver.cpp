/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>
#include <QTextStream>
#include <QDomDocument>
#include <QStringBuilder>
#include <QApplication>

#include "libGitWrap/GitWrap.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "App/MgvPrimaryWindow.hpp"

#include "libMacGitverCore/Config/Config.h"

#include "MacGitver/Modules.h"
#include "MacGitver/FSWatcher.h"
#include "MacGitver/RepoManager.hpp"


MacGitver::MacGitver()
    : mModules( NULL )
    , mLog( NULL )
    , mWatcher( NULL )
    , mRepoMan( NULL )
{
    QApplication::setOrganizationName( QLatin1String( "SaCu" ) );
    QApplication::setApplicationName( QLatin1String( "MacGitver" ) );

    mWatcher = new FSWatcher( this );

    mModules = new Modules( this );

    mRepoMan = new RepoManager( this );

    Q_ASSERT( sSelf == NULL );
    sSelf = this;

    // Continue with the rest of the init-process after QApplication::exec() has started to run.
    QMetaObject::invokeMethod( this, "boot", Qt::QueuedConnection );
}

void MacGitver::boot()
{
    loadLevels();
    loadModules();

    MgvPrimaryWindow* pw = new MgvPrimaryWindow;
    pw->show();
}

MacGitver::~MacGitver()
{
    setRepository( Git::Repository() );

    delete mRepoMan;
    delete mModules;
    delete mLog;
    sSelf = NULL;
}

Git::Repository MacGitver::repository() const
{
    return mRepository;
}

void MacGitver::setRepository( const Git::Repository& repo )
{
    mRepository = repo;

    mModules->repositoryChanged( repo );

    emit repositoryChanged( mRepository );
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

Modules* MacGitver::modules()
{
    return mModules;
}

RepoManager& MacGitver::repoMan()
{
    return *self().mRepoMan;
}

FSWatcher* MacGitver::watcher()
{
    return mWatcher;
}

CoreLog* MacGitver::log()
{
    if( !mLog )
    {
        mLog = new CoreLog();
    }
    return mLog;
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

void MacGitver::searchModules( const QDir& binDir )
{
    QStringList modFiles;
    modFiles << QLatin1String( "Mod*.mgv" );

    qDebug( "Searching for Modules in %s", qPrintable( binDir.absolutePath() ) );

    foreach( QString modName, binDir.entryList( modFiles ) )
    {
        QPluginLoader* loader = new QPluginLoader( binDir.filePath( modName ), this );
        QObject* o = loader->instance();
        if( !o )
            qDebug( "%s: %s", qPrintable( modName ), qPrintable( loader->errorString() ) );

        Module* mod = qobject_cast< Module* >( o );
        if( mod )
        {
            modules()->addModule( mod );
        }
        else
        {
            delete loader;
        }
    }

}

void MacGitver::loadModules()
{
    QDir binDir( qApp->applicationDirPath() );
    searchModules( binDir );

    binDir = QDir( qApp->applicationDirPath() % QLatin1String( "/modules" ) );
    searchModules( binDir );
    binDir.cdUp();

    #ifdef Q_OS_UNIX
    binDir = QDir( qApp->applicationDirPath() % QLatin1String( "/../libexec/MacGitver/modules" ) );
    searchModules( binDir );
    #endif

    mModules->initialize();
}

void MacGitver::loadLevels()
{
    Config::self().loadLevels( QLatin1String( ":/Xml/Levels.xml" ) );
}
