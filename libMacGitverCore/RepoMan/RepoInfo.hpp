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

#ifndef MGV_REPOSITORY_INFO_HPP
#define MGV_REPOSITORY_INFO_HPP

#include <QObject>

class QTimer;

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/MacGitverApi.hpp"

class MGV_CORE_API Repo : public QObject
{
    Q_OBJECT
public:
    typedef QList< Repo* > List;

public:
    Repo();
    Repo( const Git::Repository& repo );
    ~Repo();

public:
    Git::Repository gitRepo();

    bool isSubModule() const;
    bool isBare() const;
    bool isLoaded() const;
    bool isDisabled() const;
    bool isActive() const;

    Repo* parentRepository();
    List children() const;

    void setActive( bool active );

    QString path() const;

    QString displayAlias() const;
    void setDisplayAlias( const QString& alias );

    QString branchDisplay() const;

    void close();

    Repo* repoByPath( const QString& basePath, bool searchSubmodules );
    void scanSubmodules();

private:
    void load();
    void unload();
    void findAlias();
    bool ensureIsLoaded();
    void removeChild( Repo* child );

private slots:
    void unloadTimer();

signals:
    void aboutToClose( Repo* repo );
    void aboutToUnload( Repo* repo );
    void unloaded( Repo* repo );
    void loaded( Repo* repo );
    void childRemoved( Repo* parent, Repo* child );
    void childAdded( Repo* parent, Repo* child );
    void aliasChanged( const QString& newAlias );

private:
    Git::Repository     mRepo;                  //!< GitWrap-Repo, if loaded
    QString             mPath;                  //!< Full, absolute path to this repository
    QString             mDisplayAlias;          //!< An alias for display (Default to last path comp.)
    Repo*     mParent;                //!< This subModule-Repo's parent repository
    List                mChildren;              //!< This repo's direct submodule repositories
    bool                mIsSubModule    : 1;    //!< This is a submodule of another repo
    bool                mIsBare         : 1;    //!< This is a bare repo
    bool                mIsLoaded       : 1;    //!< This repo is currently loaded (by gitWrap)
    bool                mIsDisabled     : 1;    //!< We should not try to load this one
    bool                mIsActive       : 1;    //!< This is MGV's current active repo?
    QTimer*             mUnloadTimer;           //!< NULL or a timer to unload this repository
};

#endif
