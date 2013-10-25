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

#include "libHeaven/Icons/IconManager.hpp"
#include "libHeaven/Icons/IconDefaultProvider.hpp"

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Config/RepoManConfigPage.hpp"
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

MacGitverPrivate::MacGitverPrivate(MacGitver* owner, bool runGui)
{
    // These are used to accquire global settings and stuff...
    // Set them differently, so we can run unit tests without fiddeling about the global settings.
    if (runGui) {
        QApplication::setOrganizationName( QLatin1String( "MacGitver" ) );
        QApplication::setApplicationName( QLatin1String( "MacGitver" ) );

        Heaven::IconManager::self().defaultProvider()->addSearchPath(QLatin1String(":/Images"));
    } else {
        QApplication::setOrganizationName( QLatin1String( "MacGitver" ) );
        QApplication::setApplicationName( QLatin1String( "MacGitver_NonGui" ) );
    }

    sSelf       = owner;
    sLog        = Log::Manager::create();
    sRepoMan    = new RM::RepoMan;
    sModules    = new Modules;

    if (runGui) {
        // Continue with the rest of the init-process after QApplication::exec() has started to run.
        QMetaObject::invokeMethod( this, "bootGui", Qt::QueuedConnection );
    }
}

MacGitverPrivate::~MacGitverPrivate()
{
    unregisterGlobalConfigPages();

    delete sRepoMan;    sRepoMan    = NULL;
    delete sModules;    sModules    = NULL;
    sLog    = Log::Manager();

    sSelf = NULL;
}

void MacGitverPrivate::bootGui()
{
    registerGlobalConfigPages();
    loadLevels();
    sModules->initialize();

    new MgvPrimaryWindow;
}

MacGitver*      MacGitverPrivate::sSelf         = NULL;
RM::RepoMan*    MacGitverPrivate::sRepoMan      = NULL;
Log::Manager    MacGitverPrivate::sLog;
Modules*        MacGitverPrivate::sModules      = NULL;

MacGitver& MacGitver::self()
{
    // This assert ensures, a MacGitver instance is actually existing.
    Q_ASSERT(MacGitverPrivate::sSelf);

    return *MacGitverPrivate::sSelf;
}

RM::RepoMan& MacGitver::repoMan()
{
    Q_ASSERT(MacGitverPrivate::sRepoMan);

    return *MacGitverPrivate::sRepoMan;
}

Log::Manager MacGitver::log()
{
    return MacGitverPrivate::sLog;
}

MacGitver::MacGitver(bool runGui)
    : d(new MacGitverPrivate(this, runGui))
{
    // We've run through MacGitverPrivate's constructor, so this should be true unless there
    // was already a MacGitver instance around.
    Q_ASSERT(MacGitverPrivate::sSelf == this);
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
    log().addMessage(logMessage, type);
}

void MacGitver::log( Log::Type type, const char* logMessage )
{
    log().addMessage(QString::fromUtf8(logMessage), type);
}

void MacGitver::log( Log::Type type, const Git::Result& r, const char* logMessage )
{
    if( logMessage )
    {
        log().addMessage(QString::fromUtf8("GitWrap-Error: %1\n(%2)")
                         .arg(r.errorText()).arg(QLatin1String(logMessage)), type);
    }
    else
    {
        log().addMessage(QString::fromUtf8("GitWrap-Error: %1").arg(r.errorText()), type);
    }
}

void MacGitverPrivate::loadLevels()
{
    Config::self().loadLevels( QLatin1String( ":/Xml/Levels.xml" ) );
}

void MacGitverPrivate::registerGlobalConfigPages()
{
    GeneralConfigPage::registerPage();
    RepoManConfigPage::registerPage();
}

void MacGitverPrivate::unregisterGlobalConfigPages()
{
    GeneralConfigPage::unregisterPage();
    RepoManConfigPage::unregisterPage();
}

int MacGitver::exec()
{
    MacGitver mgv;
    return qApp->exec();
}

