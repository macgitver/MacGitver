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

#ifndef MGV_MACGITVER_PRIVATE_HPP
#define MGV_MACGITVER_PRIVATE_HPP

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libBlueSky/Application.hpp"

#include "MgvPrimaryWindow.hpp"

class QDir;

class Modules;

struct MgvViewInfo
{
    BlueSky::ViewIdentifier mIdentifier;
    QString                 mDisplayName;
    MgvViewCreator*         mCreator;
};

class MgvApp : public BlueSky::Application {
public:
    BlueSky::PrimaryWindow* newPrimaryWindow() { return new MgvPrimaryWindow; }
};

class MacGitverPrivate : public QObject
{
    Q_OBJECT
public:
    MacGitverPrivate(MacGitver *owner, bool runGui = true);
    ~MacGitverPrivate();

public:
    void init();
    void searchModules(const QDir& binDir);

private slots:
    void bootGui();

private:
    void registerGlobalConfigPages();
    void unregisterGlobalConfigPages();

public:
    Git::GitWrap        mGitWrap;
    bool                isGui;
    MgvApp              bsApp;
    static MacGitver*   sSelf;
    static Modules*     sModules;
    static Log::Manager sLog;
    static RM::RepoMan* sRepoMan;
};

#endif
