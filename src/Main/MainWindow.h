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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

#include "libGitWrap/Repository.h"

#include "Config/UserLevelDefinition.h"

#include "libHeaven/App/PrimaryWindow.hpp"

#include "libHeaven/Views/TopLevelWidget.h"

#include "libHeaven/Widgets/ModeSwitchWidget.h"

#include "Interfaces/IMainWindow.h"

#include "hic_MainWindowActions.h"

class MainWindow : public Heaven::PrimaryWindow, public IMainWindow, private MainWindowActions
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private:
    void setupUi();

public:
    void setHeadLabel();

public slots:
    void repositoryChanged( const Git::Repository& repo );

private slots:
    void onHelpAbout();
    void onToolsPreferences();
    void setupFonts();

public:
    void integrateView( Heaven::View* view, Heaven::Positions position );
    void activateMode( const QString& modeName );
    void activateLevel( UserLevelDefinition::Ptr uld );
    QWidget* widget();
    void addError( const QString& err );

private:
    void moveToCenter();
    void activateModeForRepo();
    void createPartialLayout( Heaven::ViewContainer* parent,
                              UserLevelDefaultLayoutEntry::Ptr entry );

private:
    QLabel*                     mLblCurrentBranch;
    UserLevelDefinition::Ptr    mCurrentLevel;
    Git::Repository             mRepo;
};

#endif
