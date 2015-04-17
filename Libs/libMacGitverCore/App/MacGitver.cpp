/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#include <QDebug>
#include <QTextStream>
#include <QDomDocument>
#include <QStringBuilder>
#include <QApplication>

#include "libGitWrap/GitWrap.hpp"

#include "libHeavenIcons/IconManager.hpp"
#include "libHeavenIcons/IconDefaultProvider.hpp"

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Config/RepoManConfigPage.hpp"
#include "libMacGitverCore/MacGitver/AutoRefresher.hpp"

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
    : isGui(runGui)
    , refresher(nullptr)
{
    sSelf = owner;
}

void MacGitverPrivate::init()
{
    // These are used to accquire global settings and stuff...
    // Set them differently, so we can run unit tests without fiddeling about the global settings.
    if (isGui) {
        QApplication::setOrganizationName(QStringLiteral("MacGitver"));
        QApplication::setApplicationName(QStringLiteral("MacGitver"));

        // only start the auto refresher if we're running a GUI
        refresher = new AutoRefresher;

        Heaven::IconManager::self().defaultProvider()->addSearchPath(QStringLiteral(":/Images"));
    } else {
        QApplication::setOrganizationName(QStringLiteral("MacGitver"));
        QApplication::setApplicationName(QStringLiteral("MacGitver_NonGui"));
    }

    RM::RepoMan::instance();
    sModules    = new Modules;

    if (isGui) {
        // Continue with the rest of the init-process after QApplication::exec() has started to run.
        QMetaObject::invokeMethod(this, "bootGui", Qt::QueuedConnection);
    }
}

MacGitverPrivate::~MacGitverPrivate()
{
    unregisterGlobalConfigPages();

    RM::RepoMan::instance().terminate();

    delete sModules;    sModules    = NULL;

    Log::Manager::release();

    sSelf = NULL;
}

void MacGitverPrivate::bootGui()
{
    registerGlobalConfigPages();
    sModules->initialize();

    // Create the primary window by explicitly requesting it
    bsApp.primaryWindow();
}

MacGitver*      MacGitverPrivate::sSelf         = NULL;
Modules*        MacGitverPrivate::sModules      = NULL;

MacGitver& MacGitver::self()
{
    // This assert ensures, a MacGitver instance is actually existing.
    Q_ASSERT(MacGitverPrivate::sSelf);

    return *MacGitverPrivate::sSelf;
}

RM::RepoMan& MacGitver::repoMan()
{
    return RM::RepoMan::instance();
}

MacGitver::MacGitver(bool runGui)
    : d(new MacGitverPrivate(this, runGui))
{
    d->init();

    // We've run through MacGitverPrivate's constructor, so this should be true unless there
    // was already a MacGitver instance around.
    Q_ASSERT(MacGitverPrivate::sSelf == this);
}

MacGitver::~MacGitver()
{
    delete d;
}


/**
 * @brief       Register a View to the application.
 *
 * @param[in]   identifier  The key to access this module's view (must be unique)
 *
 * @param[in]   displayName A translated name that is displayed to the user when this view is
 *              referenced.
 *
 * @param[in]   creator     A call back function that actually creates a View of this type.
 *
 * @return      The descriptor for the new view.
 *
 */
BlueSky::ViewDescriptor* MacGitver::registerView(const BlueSky::ViewIdentifier& identifier,
                                                 const QString &displayName,
                                                 MgvViewCreator creator)
{
    return new BlueSky::ViewDescriptor(identifier, displayName, creator);
}

void MacGitver::unregisterView(const BlueSky::ViewIdentifier& identifier)
{
    BlueSky::ViewDescriptor* vd = BlueSky::ViewDescriptor::get(identifier);
    if (vd) {
        vd->unregister();
    }
}

void MacGitver::log(Log::Type type, const QString& logMessage)
{
    Log::Manager().addMessage(logMessage, type);
}

void MacGitver::log(Log::Type type, const char* logMessage)
{
    Log::Manager().addMessage(QString::fromUtf8(logMessage), type);
}

void MacGitver::log(Log::Type type, const Git::Result& r, const char* logMessage)
{
    if (logMessage) {
        Log::Manager().addMessage(tr("GitWrap-Error: %1\n(%2)")
                                  .arg(r.errorText()).arg(QString::fromUtf8(logMessage)), type);
    }
    else {
        Log::Manager().addMessage(tr("GitWrap-Error: %1").arg(r.errorText()), type);
    }
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

bool MacGitver::isRunningGui() const
{
    return d->isGui;
}

AutoRefresher* MacGitver::refresher() const
{
    return d->refresher;
}
