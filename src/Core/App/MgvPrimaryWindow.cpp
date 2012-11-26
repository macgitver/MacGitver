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


#include "libGitWrap/ObjectId.h"
#include "libGitWrap/Reference.h"

#include "libHeaven/App/Application.hpp"
#include "libHeaven/Views/Mode.h"
#include "libHeaven/Widgets/FooterWidget.hpp"

#include "App/MacGitver.hpp"
#include "App/MgvPrimaryWindow.hpp"
#include "MacGitver/Modules.h"
#include "Config/Config.h"
#include "Config/Ui/ConfigDialog.hpp"
#include "Config/Ui/GeneralConfigPage.hpp"
#include "Widgets/RepoStateWidget.hpp"

#include "ui_AboutDlg.h"

MgvPrimaryWindow::MgvPrimaryWindow()
    : Heaven::PrimaryWindow()
    , mRepo()
{
    setupUi();

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
}

void MgvPrimaryWindow::setupUi()
{
    QIcon icon( QLatin1String( ":/mgv_sak32.png" ) );
    setWindowIcon( icon );

    setupActions( this );
    setMenuBar( mbMainMenuBar );

    setWindowTitle( trUtf8( "MacGitver" ) );

    statusBar()->addWidget( new RepoStateWidget );

    moveToCenter();
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
    QStringList modeNames;

    foreach( UserLevelMode::Ptr mode, uld->allModes() )
    {
        Heaven::Mode* heavenMode = new Heaven::Mode( mode->name(), NULL );
        Heaven::app()->addMode( heavenMode );
        modeNames.append( mode->name() );
    }

    mCurrentLevel = uld;

    activateModeForRepo();
}

void MgvPrimaryWindow::activateModeForRepo()
{
    QString preset = QLatin1String( mRepo.isValid() ? "Normal" : "Welcome" );

    QString configPath = mCurrentLevel->id() % QChar( L'/' ) % preset;
    QString modeName = Config::self().get( configPath, mCurrentLevel->preset( preset ) ).toString();

    activateMode( modeName );
}

void MgvPrimaryWindow::createPartialLayout( Heaven::ViewContainer* parent,
                                      UserLevelDefaultLayoutEntry::Ptr entry )
{
    switch( entry->type() )
    {
    case UserLevelDefaultLayoutEntry::Tab:
        {
            Heaven::ViewContainer::Type subType = Heaven::ViewContainer::Type( 0 );
            switch( entry->tabPos() )
            {
            case UserLevelDefaultLayoutEntry::Left:
                subType = Heaven::ViewContainer::SubTabLeft;
                break;

            case UserLevelDefaultLayoutEntry::Right:
                subType = Heaven::ViewContainer::SubTabRight;
                break;

            case UserLevelDefaultLayoutEntry::Top:
                subType = Heaven::ViewContainer::SubTabTop;
                break;

            case UserLevelDefaultLayoutEntry::Bottom:
                subType = Heaven::ViewContainer::SubTabBottom;
                break;
            }

            Heaven::ViewContainer* child = new Heaven::ViewContainer(
                                               Heaven::ViewContainer::MultiBar,
                                               subType,
                                               parent );

            parent->addContainer( child );

            foreach( UserLevelDefaultLayoutEntry::Ptr subEntry, entry->children() )
            {
                createPartialLayout( child, subEntry );
            }
        }
        break;

    case UserLevelDefaultLayoutEntry::Split:
        {
            Heaven::ViewContainer* child = new Heaven::ViewContainer(
                                               Heaven::ViewContainer::Splitter,
                                               entry->isVertical() ?
                                                   Heaven::ViewContainer::SubSplitVert :
                                                   Heaven::ViewContainer::SubSplitHorz,
                                               parent );

            parent->addContainer( child );

            foreach( UserLevelDefaultLayoutEntry::Ptr subEntry, entry->children() )
            {
                createPartialLayout( child, subEntry );
            }
        }
        break;

    case UserLevelDefaultLayoutEntry::View:
        {
            Heaven::View* v = MacGitver::self().createView( entry->name() );

            if( !v )
            {
                qDebug( "Could not create a view with id '%s'", qPrintable( entry->name() ) );
                break;
            }

            parent->addView( v );
        }
        break;
    }
}

void MgvPrimaryWindow::activateMode( const QString& modeName )
{
    qDebug( "Going to %s mode", qPrintable( modeName ) );

    // Will just update the display for now
    Heaven::app()->setCurrentMode( Heaven::app()->findMode( modeName ) );

    topLevelContainer()->clear();

    UserLevelMode::Ptr mode = mCurrentLevel->mode( modeName );

    UserLevelDefaultLayout::Ptr layout = mode->defaultLayout();

    createPartialLayout( topLevelContainer()->rootContainer(), layout->root() );
}

void MgvPrimaryWindow::repositoryChanged( const Git::Repository& repo )
{
    mRepo = repo;
    activateModeForRepo();
}

void MgvPrimaryWindow::integrateView( Heaven::View* view, Heaven::Positions position )
{
    topLevelContainer()->addView( view, position );
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
    MacGitver::self().modules()->setupConfigPages( &d );
    d.exec();
}
