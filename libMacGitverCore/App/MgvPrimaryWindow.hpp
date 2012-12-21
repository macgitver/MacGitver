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

#ifndef MGV_PRIMARY_WINDOW_HPP
#define MGV_PRIMARY_WINDOW_HPP

#include "libGitWrap/Repository.hpp"

#include "libHeaven/Views/View.h"       // Remove this along with the method that requires it!
#include "libHeaven/App/PrimaryWindow.hpp"

#include "Config/UserLevelDefinition.h"

#include "hic_MgvPrimaryWindowActions.h"

class MgvPrimaryWindowPrivate;

class MgvPrimaryWindow : public Heaven::PrimaryWindow, private MgvPrimaryWindowActions
{
    Q_OBJECT
public:
    MgvPrimaryWindow();
    ~MgvPrimaryWindow();

private:
    void setupUi();

public slots:
    void repositoryChanged( const Git::Repository& repo );

private slots:
    void onQuit();
    void onHelpAbout();
    void onToolsPreferences();
    void onViewRefresh();
    void setupFonts();

public:
    void integrateView( Heaven::View* view, Heaven::Positions position );
    void activateMode( const QString& modeName );
    void activateLevel( UserLevelDefinition::Ptr uld );
    void addError( const QString& err );

private:
    void savePosition();
    void moveToCenter();
    void activateModeForRepo();
    void createPartialLayout( Heaven::ViewContainer* parent,
                              UserLevelDefaultLayoutEntry::Ptr entry );

protected:
    void closeEvent( QCloseEvent* ev );

private:
    MgvPrimaryWindowPrivate* d;
};

#endif
