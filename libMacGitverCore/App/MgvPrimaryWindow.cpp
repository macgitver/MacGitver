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

#include "libHeaven/App/Application.hpp"
#include "libHeaven/Views/Mode.h"
#include "libHeaven/Views/TopLevelWidget.h"
#include "libHeaven/Widgets/FooterWidget.hpp"

#include "libMacGitverCore/App/MacGitverPrivate.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindow.hpp"
#include "libMacGitverCore/App/MgvPrimaryWindowPrivate.hpp"
#include "libMacGitverCore/MacGitver/Modules.h"
#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/Config/Ui/ConfigDialog.hpp"
#include "libMacGitverCore/Config/Ui/GeneralConfigPage.hpp"
#include "libMacGitverCore/Widgets/RepoStateWidget.hpp"

#include "ui_AboutDlg.h"

MgvPrimaryWindowPrivate::MgvPrimaryWindowPrivate()
    : currentLevel()
    , repo()
{
}

MgvPrimaryWindow::MgvPrimaryWindow()
    : Heaven::PrimaryWindow()
{
    d = new MgvPrimaryWindowPrivate;
    setupUi();

    Heaven::app()->setViewFactory( MacGitver::self().viewFactory() );

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             SLOT(repositoryChanged(Git::Repository)) );

    QString levelId = Config::self().get( "UserLevel", "Novice" ).toString();

    foreach( UserLevelDefinition::Ptr uld, Config::self().levels() )
    {
        if( uld->id() == levelId )
        {
            activateLevel( uld );
            break;
        }
    }

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

    setWindowTitle( trUtf8( "MacGitver" ) );

    statusBar()->addWidget( new RepoStateWidget );

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
    Heaven::PrimaryWindow::closeEvent( ev );
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

void MgvPrimaryWindow::activateLevel( UserLevelDefinition::Ptr uld )
{
    d->currentLevel = uld;

    activateModeForRepo();
}

void MgvPrimaryWindow::activateModeForRepo()
{
    QString preset = QLatin1String( d->repo.isValid() ? "Normal" : "Welcome" );

    QString configPath = d->currentLevel->id() % QChar( L'/' ) % preset;
    QString modeName = Config::self().get( configPath, d->currentLevel->preset( preset ) ).toString();

    activateMode( d->currentLevel->name() % QChar( L'#' ) % modeName );
}

void MgvPrimaryWindow::activateMode( const QString& modeName )
{
    qDebug( "Going to %s mode", qPrintable( modeName ) );
    Heaven::app()->setCurrentMode( Heaven::app()->findMode( modeName ) );
}

void MgvPrimaryWindow::repositoryChanged( const Git::Repository& repo )
{
    d->repo = repo;
    activateModeForRepo();
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
    d.addPage( new GeneralConfigPage( &d ) );
    MacGitverPrivate::sModules->setupConfigPages( &d );
    d.exec();
}

void MgvPrimaryWindow::onViewRefresh()
{
}
