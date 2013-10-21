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
#include <QTextStream>
#include <QDomDocument>
#include <QStringBuilder>
#include <QApplication>

#include "libGitWrap/GitWrap.hpp"

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"

#include "libMacGitverCore/Log/LogManager.hpp"

/**
 * @class   MacGitver
 * @brief   Central core of the MacGitver application
 *
 * The singleton MacGitver is the central core of the application. It's main purpose is to provide
 * access to various singleton classes that manage the system. These are:
 *
 * - RepoManager \n
 *   The repository manager can be obtained via a call to MacGitver::repoMan(). It is responsible
 *   for managing the currently open repositories and which one of them is active.
 *
 * - CoreLog \n
 *   The core log can be accessed via MacGitver::log(). It keeps track of debugging information and
 *   provides the ability to inform the user of what actions have happened.
 *
 */

MacGitverPrivate::MacGitverPrivate( MacGitver* owner )
{
    QApplication::setOrganizationName( QLatin1String( "MacGitver" ) );
    QApplication::setApplicationName( QLatin1String( "MacGitver" ) );

    sSelf       = owner;
    sLog        = Log::Manager::create();
    sRepoMan    = new RepoManager;
    sModules    = new Modules;

    // Continue with the rest of the init-process after QApplication::exec() has started to run.
    QMetaObject::invokeMethod( this, "boot", Qt::QueuedConnection );
}

MacGitverPrivate::~MacGitverPrivate()
{
    unregisterGlobalConfigPages();

    delete sModules;    sModules    = NULL;
    delete sRepoMan;    sRepoMan    = NULL;
    sLog    = Log::Manager();

    sSelf = NULL;
}

void MacGitverPrivate::boot()
{
    registerGlobalConfigPages();
    loadLevels();
    sModules->initialize();

    new MgvPrimaryWindow;
}

MacGitver*      MacGitverPrivate::sSelf         = NULL;
RepoManager*    MacGitverPrivate::sRepoMan      = NULL;
Log::Manager    MacGitverPrivate::sLog;
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

Log::Manager MacGitver::log()
{
    return MacGitverPrivate::sLog;
}

MacGitver::MacGitver()
    : d( new MacGitverPrivate( this ) )
{
    Q_ASSERT( MacGitverPrivate::sSelf == this );
}

MacGitver::~MacGitver()
{
    delete d;
}

void MacGitver::registerView( const Heaven::ViewIdentifier& identifier, const QString &displayName,
                              MgvViewCreator creator )
{
    new Heaven::ViewDescriptor( identifier, displayName, creator );
}

void MacGitver::unregisterView( const Heaven::ViewIdentifier& identifier )
{
    Heaven::ViewDescriptor* vd = Heaven::ViewDescriptor::get( identifier );
    if( vd )
    {
        vd->unregister();
    }
}

void MacGitver::log( Log::Type type, const QString& logMessage )
{
    log().addMessage( type, logMessage );
}

void MacGitver::log( Log::Type type, const char* logMessage )
{
    log().addMessage( type, QString::fromUtf8( logMessage ) );
}

void MacGitver::log( Log::Type type, const Git::Result& r, const char* logMessage )
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

void MacGitverPrivate::loadLevels()
{
    Config::self().loadLevels( QLatin1String( ":/Xml/Levels.xml" ) );
}

void MacGitverPrivate::registerGlobalConfigPages()
{
    GeneralConfigPage::registerPage();
}

void MacGitverPrivate::unregisterGlobalConfigPages()
{
    GeneralConfigPage::unregisterPage();
}

int MacGitver::exec()
{
    MacGitver mgv;
    return qApp->exec();
}

