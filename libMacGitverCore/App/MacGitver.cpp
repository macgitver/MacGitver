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

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/MacGitver/FSWatcher.h"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"

MacGitverPrivate::MacGitverPrivate( MacGitver* owner )
{
    QApplication::setOrganizationName( QLatin1String( "MacGitver" ) );
    QApplication::setApplicationName( QLatin1String( "MacGitver" ) );

    sSelf       = owner;
    sLog        = new CoreLog;
    sRepoMan    = new RepoManager;
    sModules    = new Modules;

    // Continue with the rest of the init-process after QApplication::exec() has started to run.
    QMetaObject::invokeMethod( this, "boot", Qt::QueuedConnection );
}

MacGitverPrivate::~MacGitverPrivate()
{
    delete sModules;    sModules    = NULL;
    delete sRepoMan;    sRepoMan    = NULL;
    delete sLog;        sLog        = NULL;

    sSelf = NULL;
}

void MacGitverPrivate::boot()
{
    loadLevels();
    loadModules();

    MgvPrimaryWindow* pw = new MgvPrimaryWindow;
    pw->show();
}

MacGitver*      MacGitverPrivate::sSelf         = NULL;
RepoManager*    MacGitverPrivate::sRepoMan      = NULL;
CoreLog*        MacGitverPrivate::sLog          = NULL;
Modules*        MacGitverPrivate::sModules      = NULL;

MacGitver& MacGitver::self()
{
    Q_ASSERT( MacGitverPrivate::sSelf );  /* exec() must have been called */
    return *MacGitverPrivate::sSelf;
}

RepoManager& MacGitver::repoMan()
{
    return *MacGitverPrivate::sRepoMan;
}

CoreLog& MacGitver::log()
{
    return *MacGitverPrivate::sLog;
}

MacGitver::MacGitver()
    : d( new MacGitverPrivate( this ) )
{
    Q_ASSERT( MacGitverPrivate::sSelf == this );
}

MacGitver::~MacGitver()
{
    setRepository( Git::Repository() );
    delete d;
}

Git::Repository MacGitver::repository() const
{
    return d->mRepository;
}

void MacGitver::setRepository( const Git::Repository& repo )
{
    d->mRepository = repo;

    MacGitverPrivate::sModules->repositoryChanged( repo );

    emit repositoryChanged( repo );
}

void MacGitver::registerView( const QString& identifier, Heaven::ViewTypes type,
                              MgvViewCreator* creator )
{
    Q_ASSERT( !d->mViews.contains( identifier ) );

    MgvViewInfo vi;
    vi.mIdentifier = identifier;
    vi.mType = type;
    vi.mCreator = creator;
    d->mViews.insert( identifier, vi );
}

void MacGitver::unregisterView( const QString& identifier )
{
    d->mViews.remove( identifier );
}

Heaven::View* MacGitver::createView( const QString& identifier )
{
    if( d->mViews.contains( identifier ) )
    {
        MgvViewInfo vi = d->mViews.value( identifier );
        return vi.mCreator();
    }

    return NULL;
}

void MacGitver::log( LogType type, const QString& logMessage )
{
    log().addMessage( type, logMessage );
}

void MacGitver::log( LogType type, const char* logMessage )
{
    log().addMessage( type, QString::fromUtf8( logMessage ) );
}

void MacGitver::log( LogType type, const Git::Result& r, const char* logMessage )
{
    if( logMessage )
    {
        log().addMessage( type, QString::fromUtf8( "GitWrap-Error: %1\n(%2)" )
                          .arg( r.errorText() ).arg( QLatin1String( logMessage ) ) );
    }
    else
    {
        log().addMessage( type, QString::fromUtf8( "GitWrap-Error: %1" )
                          .arg( r.errorText() ) );
    }
}

void MacGitverPrivate::searchModules( const QDir& binDir )
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
            sModules->addModule( mod );
        }
        else
        {
            delete loader;
        }
    }

}

void MacGitverPrivate::loadModules()
{
    QDir binDir( qApp->applicationDirPath() );
    searchModules( binDir );

    binDir = QDir( qApp->applicationDirPath() % QLatin1Literal( "/modules" ) );
    searchModules( binDir );
    binDir.cdUp();

    #ifdef Q_OS_UNIX
    binDir = QDir( qApp->applicationDirPath() % QLatin1Literal( "/../libexec/MacGitver/modules" ) );
    searchModules( binDir );
    #endif

    sModules->initialize();
}

void MacGitverPrivate::loadLevels()
{
    Config::self().loadLevels( QLatin1String( ":/Xml/Levels.xml" ) );
}

int MacGitver::exec()
{
    MacGitver mgv;
    return qApp->exec();
}
