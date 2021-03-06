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

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindowPrivate.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"

#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/Widgets/RepoStateWidget.hpp"

#include "libBlueSky/Application.hpp"
#include "libBlueSky/FooterWidget.hpp"

#include <QDebug>
#include <QComboBox>
#include <QStatusBar>
#include <QFile>
#include <QApplication>
#include <QMenuBar>
#include <QDockWidget>
#include <QStringBuilder>
#include <QDesktopWidget>
#include <QApplication>

#if 0
#include "libStairway/StairwayToHeavenTool.hpp"
#endif

#include "ui_AboutDlg.h"

MgvPrimaryWindowPrivate::MgvPrimaryWindowPrivate()
{
}

MgvPrimaryWindow::MgvPrimaryWindow()
    : BlueSky::PrimaryWindow()
    , d( new MgvPrimaryWindowPrivate )
{
    setupUi();

    #if 0
    Heaven::StairwayTool::StairwayView* w = new Heaven::StairwayTool::StairwayView();
    w->setRoot(this);
    w->show();
    #endif

    connect(&MacGitver::repoMan(), &RM::RepoMan::firstRepositoryOpened,
            this, &MgvPrimaryWindow::activateModeForRepo);

    connect(&MacGitver::repoMan(), &RM::RepoMan::lastRepositoryClosed,
            this, &MgvPrimaryWindow::activateModeForRepo);

    setupFonts();

    connect( &Config::self(), SIGNAL(fontsChanged()),
             this, SLOT(setupFonts()) );
}

MgvPrimaryWindow::~MgvPrimaryWindow()
{
    delete d;
}

void MgvPrimaryWindow::setupUi()
{
    QIcon icon( QStringLiteral( ":/mgv_sak32.png" ) );
    setWindowIcon( icon );

    setupActions( this );
    setMenuBar( mbMainMenuBar );

    BlueSky::ViewDescriptor::mergeViewsMenu("ViewsListMP");

    setWindowTitle( trUtf8( "MacGitver" ) );

    footer()->addWidget(new RepoStateWidget);
    QMetaObject::invokeMethod(this, "showLater", Qt::QueuedConnection);
}

void MgvPrimaryWindow::showLater()
{
    if (!restoreGeometry(Config::self().get("Geometry").toByteArray())) {
        moveToCenter();
    }

    // Having the internal geometry calculated in advance seems to help Mac OS X a little bit to
    // avoid unwanted resize-animations.
    updateGeometry();

    if (Config::self().get("FullScreen").toBool()) {
        showFullScreen();
    }
    else {
        show();
    }
}

void MgvPrimaryWindow::savePosition()
{
    Config& c(Config::self());

    c.set("FullScreen", isFullScreen());
    c.set("Geometry", saveGeometry());

    c.saveSettings();
}

void MgvPrimaryWindow::closeEvent( QCloseEvent* ev )
{
    savePosition();
    BlueSky::PrimaryWindow::closeEvent( ev );
}

void MgvPrimaryWindow::onQuit()
{
    savePosition();
    qApp->quit();
}

void MgvPrimaryWindow::setupFonts()
{
    QFont font = Config::defaultFont();
    setFont( font );
    QApplication::setFont( font );
    QApplication::setFont( Config::defaultDialogFont(), "QDialog" );
}

void MgvPrimaryWindow::moveToCenter()
{
    QRect desk = QApplication::desktop()->availableGeometry();

    QRect center = QRect( desk.width() / 12, desk.height() / 12,
                          desk.width() - desk.width() / 6,
                          desk.height() - desk.height() / 6 );
    setGeometry( center );
}

void MgvPrimaryWindow::activateModeForRepo()
{
    if (MacGitver::repoMan().repositories().count() > 0) {
        activateMode(QStringLiteral("HistoryMode"));
    }
    else {
        activateMode(QString());
    }
}

void MgvPrimaryWindow::activateMode( const QString& modeName )
{
    if (modeName.isEmpty()) {
        qDebug() << "Going to mode <NONE>";
    }
    else {
        qDebug() << QByteArray("Going to mode: ") % modeName.toUtf8();
    }

    BlueSky::Mode* mode = BlueSky::Application::instance()->findMode(modeName.toUtf8());
    BlueSky::Application::instance()->setActiveMode(mode);
}

void MgvPrimaryWindow::onHelpAbout()
{
    QDialog d;
    Ui::AboutDlg u;
    u.setupUi( &d );
    d.exec();
}

void MgvPrimaryWindow::onToolsPreferences()
{
    ConfigDialog d;
    d.exec();
}

void MgvPrimaryWindow::onViewRefresh()
{
}
