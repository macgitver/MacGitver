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

#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QApplication>

class QDir;

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libMacGitverCore/Interfaces/ILog.h"

#include "libGitWrap/Repository.hpp"
#include "libGitWrap/Result.hpp"

#include "libHeaven/Views/View.h"


class Module;
class Modules;
class FSWatcher;
class UserLevelDefinition;
class RepoManager;

typedef Heaven::View* (ViewCreator)( );

class MGV_CORE_API MacGitver : public QApplication
{
    Q_OBJECT
private:
    struct ViewInfo
    {
        QString             mIdentifier;
        ViewCreator*        mCreator;
        Heaven::ViewTypes   mType;
    };

public:
    MacGitver( int argc, char **argv );
    ~MacGitver();

public:
    static MacGitver& self();

public:
    void setRepository( const Git::Repository &repo );
    Git::Repository repository() const;
    Modules* modules();
    FSWatcher* watcher();

    void integrateView( Heaven::View* view, Heaven::Positions place );

    void registerView( const QString& identifier, Heaven::ViewTypes type, ViewCreator* creator );
    void unregisterView( const QString& identifier );
    Heaven::View* createView( const QString& identifier );

    ILog* log();
    void setLog( ILog* log );
    void log( LogType type, const QString& logMessage );
    void log( LogType type, const char* logMessage );
    void log( LogType type, const Git::Result& r, const char* logMessage = NULL );

    RepoManager* repoMan();
    int exec();

signals:
    void repositoryChanged( const Git::Repository& repo );

private:
    void loadModules();
    void loadLevels();
    void searchModules( const QDir& binDir );

private slots:
    void boot();

private:
    static MacGitver*           sSelf;

    Modules*                    mModules;
    ILog*                       mLog;
    FSWatcher*                  mWatcher;
    Git::GitWrap                mGitWrap;
    Git::Repository             mRepository;
    QHash< QString, ViewInfo >  mViews;
    RepoManager*                mRepoMan;
};

#endif

