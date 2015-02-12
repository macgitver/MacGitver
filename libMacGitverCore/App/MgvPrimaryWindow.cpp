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
#include <QComboBox>
#include <QStatusBar>
#include <QFile>
#include <QApplication>
#include <QMenuBar>
#include <QDockWidget>
#include <QStringBuilder>
#include <QDesktopWidget>
#include <QApplication>


#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/Reference.hpp"

#include "libBlueSky/Application.hpp"
#include "libBlueSky/FooterWidget.hpp"

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindowPrivate.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/Widgets/RepoStateWidget.hpp"

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

    connect(&MacGitver::repoMan(), SIGNAL(firstRepositoryOpened()),
            this, SLOT(activateModeForRepo()));

    connect(&MacGitver::repoMan(), SIGNAL(lastRepositoryClosed()),
            this, SLOT(activateModeForRepo()));

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
    QIcon icon( QLatin1String( ":/mgv_sak32.png" ) );
    setWindowIcon( icon );

    setupActions( this );
    setMenuBar( mbMainMenuBar );

    BlueSky::ViewDescriptor::mergeViewsMenu("ViewsListMP");

    setWindowTitle( trUtf8( "MacGitver" ) );

    const RM::RepoMan& rm = MacGitver::repoMan();
    RepoStateWidget* repoState = new RepoStateWidget;
    connect( &rm, SIGNAL(refMoved(RM::Repo*,RM::Ref*)),
             repoState, SLOT(onUpdateHEAD(RM::Repo*,RM::Ref*)) );
    footer()->addWidget( repoState );
    QMetaObject::invokeMethod(this, "showLater", Qt::QueuedConnection);
}

void MgvPrimaryWindow::showLater()
{
    if( Config::self().get( "FullScreen" ).toBool() )
    {
        // Useless on MacOSX with Qt < 5.0
        // should add a .mm file to enable the full-screen feature
        // Since: showFullScreen() at least _looks_ like clicking the full-screen icon. Maybe it's
        //        even working the same way.
        showFullScreen();
    }
    else
    {
        QRect r = Config::self().get( "Geometry" ).toRect();
        if( r.isEmpty() )
        {
            moveToCenter();
        }
        else
        {
            resize( r.size() );
            move( r.topLeft() );
        }
        show();
    }
}

void MgvPrimaryWindow::savePosition()
{
    Config& c = Config::self();
    c.set( "FullScreen", isFullScreen() );
    c.set( "Geometry", geometry() );
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
        activateMode(QLatin1String("HistoryMode"));
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
