/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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
#include <QtPlugin>

#include "libHeaven/Heaven.hpp"
#include "libHeaven/App/PrimaryWindow.hpp"

#include "libMacGitverCore/Config/Config.h"
#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "RepositoryModule.h"
#include "RepoTreeView.hpp"
#include "CloneRepositoryDlg.h"
#include "CreateRepositoryDlg.h"

RepositoryModule::RepositoryModule()
    : ConfigUser( "Repository" )
{
}

RepositoryModule::~RepositoryModule()
{
}

void RepositoryModule::setupConfigPages( ConfigDialog* dialog )
{
}

Heaven::View* RepositoryModule::createRepoTreeView()
{
    return new RepoTreeView;
}

void RepositoryModule::initialize()
{
    setupActions( this );
    acRepositoryMenuAC->mergeInto( "RepositoryMenuMP" );
    acRepositoryToolBarAC->mergeInto( "RepositoryToolBarMP" );

    connect( damRecentlyUsed, SIGNAL(entryTriggered(QVariant)),
             this, SLOT(onRecentRepositoryOpen(QVariant)) );

    mMostRecentlyUsed = configGet( "MRU", QStringList() );

    connect(&MacGitver::repoMan(),  SIGNAL(repositoryOpened(Repo*)),
            this,                   SLOT(onCoreRepoOpen(Repo*)));

    connect(&MacGitver::repoMan(),  SIGNAL(hasActiveRepositoryChanged(bool)),
            actRepositoryClose,     SLOT(setEnabled(bool)));

    updateMostRecentlyUsedMenu();

    MacGitver::self().registerView( "RepoTree",
                                    tr( "Repository" ),
                                    &RepositoryModule::createRepoTreeView );
}

void RepositoryModule::deinitialize()
{
    MacGitver::self().unregisterView( "RepoTree" );
}

void RepositoryModule::onRepositoryClose()
{
}

void RepositoryModule::onRepositoryCreate()
{
    CreateRepositoryDlg().exec();
}

void RepositoryModule::onRepositoryOpen()
{
    QWidget* parent = Heaven::primaryWindow();
    QFileDialog *fd = new QFileDialog( parent );
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

    fd->open( this, SLOT(onRepositoryOpenHelper()) );

    #ifdef Q_OS_MAC
    // workaround for osx sheets without a parent
    if (parent == 0)
        fd->exec();
    #endif
}

void RepositoryModule::onRepositoryOpenHelper()
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

    MacGitver::repoMan().open( repo );
}

void RepositoryModule::onRepositoryClone()
{
    CloneRepositoryDlg().exec();
}

void RepositoryModule::onRecentRepositoryOpen( const QVariant& path )
{
    QString repoPath = path.toString();
    MacGitver::repoMan().open( repoPath );
}

void RepositoryModule::onCoreRepoOpen( Repo* repo )
{
    if( repo->isSubModule() )
    {
        return;
    }

    QString path = repo->path();
    mMostRecentlyUsed.removeAll( path );
    mMostRecentlyUsed.prepend( path );

    configSet( "MRU", mMostRecentlyUsed );
    updateMostRecentlyUsedMenu();
}

void RepositoryModule::updateMostRecentlyUsedMenu()
{
    damRecentlyUsed->setMode( Heaven::DAMergerAdvancedList );
    damRecentlyUsed->addStringList( mMostRecentlyUsed );
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Repository, RepositoryModule )
#endif
