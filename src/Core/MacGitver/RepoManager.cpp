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

#include <QFileDialog>

#include "Config/Config.h"

#include "Interfaces/IMainWindow.h"

#include "MacGitver.h"
#include "RepoManager.hpp"

RepoManager::RepoManager( QObject* parent )
    : QObject( parent )
{
    mActiveRepo = NULL;
}

RepoManager::~RepoManager()
{
    closeAll();
}

void RepoManager::open()
{
    QWidget* main = MacGitver::self().mainWindow()->widget();

    QFileDialog *fd = new QFileDialog(main);
#ifdef Q_OS_MAC
    fd->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
#else
    fd->setFileMode(QFileDialog::Directory);
#endif

    QString lastUsedDir = Config::self().get( "Repository/lastUsedDir", "#" ).toString();
    if( lastUsedDir != QLatin1String( "#" ) )
    {
        fd->setDirectory( lastUsedDir );
    }

    fd->setWindowTitle( trUtf8("Open a git repository") );

    fd->open(this, SLOT(onRepositoryOpenHelper()));
}


void RepoManager::onRepositoryOpenHelper()
{
    QFileDialog *fd = qobject_cast<QFileDialog *>(sender());
    Q_ASSERT(fd != 0);

    if ( fd->selectedFiles().isEmpty() )
    {
        return;
    }

    //! @todo error handling
    Git::Result r;
    QString repoDir = Git::Repository::discover( fd->selectedFiles().first(), false,
                                                 QStringList(), r );
    if( repoDir.isEmpty() )
    {
        return;
    }

    //! @todo error handling
    Git::Repository repo = Git::Repository::open( repoDir, r );
    if( !repo.isValid() )
        return;

    // If we successfully loaded the repository at that directory,
    // we store the repository's work dir as "lastUsedDir".
    // If it is a bare repository, we store the repository's directory.
    Config::self().set( "Repository/lastUsedDir", repo.isBare() ? repoDir : repo.basePath() );

    open( repo );
}

void RepoManager::open( const QString& path )
{
}

void RepoManager::open( const Git::Repository& repo )
{
    RepositoryInfo* info = repoByPath( repo.basePath(), false );

    if( !info )
    {
        info = new RepositoryInfo( repo );
        mRepos.append( info );
        emit repositoryOpened( info );

        // we need to scan for submodules explicitly, since we didn't call load() on the RepoInfo
        info->scanSubmodules();
    }

    activate( info );
}

RepositoryInfo* RepoManager::repoByPath( const QString& basePath, bool searchSubmodules )
{
    foreach( RepositoryInfo* info, mRepos )
    {
        if( info->path() == basePath )
        {
            return info;
        }

        if( searchSubmodules )
        {
            if( RepositoryInfo* sub = info->repoByPath( basePath, true ) )
            {
                return sub;
            }
        }
    }

    return NULL;
}

void RepoManager::closeAll()
{
    foreach( RepositoryInfo* repo, mRepos )
    {
        repo->close();
    }
}

void RepoManager::activate( RepositoryInfo* repository )
{
    if( repository == mActiveRepo )
    {
        return;
    }

    if( mActiveRepo )
    {
        mActiveRepo->setActive( false );
    }

    if( repository )
    {
        // For compatibility
        MacGitver::self().setRepository( repository->gitRepo() );

        repository->setActive( true );
    }
    else
    {
        MacGitver::self().setRepository( Git::Repository() );
    }
}

void RepoManager::internalActivate( RepositoryInfo* repository )
{
    if( repository == mActiveRepo )
    {
        return;
    }

    mActiveRepo = repository;
    emit repositoryActivated( repository );
}

RepositoryInfo* RepoManager::activeRepository()
{
    return mActiveRepo;
}

RepositoryInfo::List RepoManager::repositories() const
{
    return mRepos;
}
