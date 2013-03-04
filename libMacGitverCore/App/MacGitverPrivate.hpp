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

class QDir;

class Modules;

struct MgvViewInfo
{
    Heaven::ViewIdentifier  mIdentifier;
    QString                 mDisplayName;
    MgvViewCreator*         mCreator;
};

typedef QHash< Heaven::ViewIdentifier, MgvViewInfo > MgvViewInfos;

class MgvViewFactory : public Heaven::ViewFactory
{
public:
    MgvViewFactory( MacGitverPrivate* _d );

    Heaven::View* createView( const Heaven::ViewIdentifier& identifier );

private:
    MacGitverPrivate* d;
};

class MacGitverPrivate : public QObject
{
    Q_OBJECT
public:
    MacGitverPrivate( MacGitver* owner );
    ~MacGitverPrivate();

public:
    void loadLevels();
    void searchModules( const QDir& binDir );

private slots:
    void boot();

public:
    Git::GitWrap        mGitWrap;
    Git::Repository     mRepository;    /* deprecated */
    MgvViewInfos        mViews;
    MgvViewFactory*     mViewFactory;

public:
    static MacGitver*   sSelf;
    static Modules*     sModules;
    static CoreLog*     sLog;
    static RepoManager* sRepoMan;
};

#endif
