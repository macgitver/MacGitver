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

#ifndef MGV_PRIMARY_WINDOW_HPP
#define MGV_PRIMARY_WINDOW_HPP

#include "libGitWrap/Repository.hpp"

#include "libBlueSky/Windows.hpp"

#include "hic_MgvPrimaryWindowActions.h"

class MgvPrimaryWindowPrivate;

class Repo;

class MgvPrimaryWindow : public BlueSky::PrimaryWindow, private MgvPrimaryWindowActions
{
    Q_OBJECT
public:
    MgvPrimaryWindow();
    ~MgvPrimaryWindow();

private:
    void setupUi();

private slots:
    void onQuit();
    void onHelpAbout();
    void onToolsPreferences();
    void onViewRefresh();
    void setupFonts();

public:
    void activateMode( const QString& modeName );

private:
    void savePosition();
    void moveToCenter();

private slots:
    void activateModeForRepo();

protected:
    void closeEvent( QCloseEvent* ev );

private:
    MgvPrimaryWindowPrivate* d;
};

#endif
