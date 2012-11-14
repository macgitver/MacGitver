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

#ifndef MGV_REPOSITORY_INFO_HPP
#define MGV_REPOSITORY_INFO_HPP

#include <QObject>

class QTimer;

#include "libGitWrap/Repository.h"

class RepositoryInfo : public QObject
{
    Q_OBJECT
public:
    typedef QList< RepositoryInfo* > List;

public:
    RepositoryInfo();
    ~RepositoryInfo();

public:
    Git::Repository gitRepo();

    bool isSubModule() const;
    bool isBare() const;
    bool isLoaded() const;
    bool isDisabled() const;
    bool isActive() const;

    RepositoryInfo* parentRepository();
    List children() const;

    void setActive( bool active );

    QString displayAlias() const;
    void setDisplayAlias( const QString& alias );

private:
    void load();
    void unload();
    bool ensureIsLoaded();
    void removeChild( RepositoryInfo* child );

private slots:
    void unloadTimer();

signals:
    void childRemoved( RepositoryInfo* parent, RepositoryInfo* child );
    void childAdded( RepositoryInfo* parent, RepositoryInfo* child );
    void aliasChanged( const QString& newAlias );

private:
    Git::Repository     mRepo;                  //!< GitWrap-Repo, if loaded
    QString             mPath;                  //!< Full, absolute path to this repository
    QString             mDisplayAlias;          //!< An alias for display (Default to last path comp.)
    RepositoryInfo*     mParent;                //!< This subModule-Repo's parent repository
    List                mChildren;              //!< This repo's direct submodule repositories
    bool                mIsSubModule    : 1;    //!< This is a submodule of another repo
    bool                mIsBare         : 1;    //!< This is a bare repo
    bool                mIsLoaded       : 1;    //!< This repo is currently loaded (by gitWrap)
    bool                mIsDisabled     : 1;    //!< We should not try to load this one
    bool                mIsActive       : 1;    //!< This is MGV's current active repo?
    QTimer*             mUnloadTimer;           //!< NULL or a timer to unload this repository
};

#endif
