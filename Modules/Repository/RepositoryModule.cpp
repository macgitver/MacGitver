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

#include "libBlueSky/Application.hpp"
#include "libBlueSky/Windows.hpp"

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

void RepositoryModule::initialize()
{
    setupActions( this );
    acRepositoryMenuAC->mergeInto( "RepositoryMenuMP" );

    connect( damRecentlyUsed, SIGNAL(entryTriggered(QVariant)),
             this, SLOT(onRecentRepositoryOpen(QVariant)) );

    mMostRecentlyUsed = configGet( "MRU", QStringList() );

    connect(&RM::RepoMan::instance(),   &RM::RepoMan::repositoryOpened,
            this,                       &RepositoryModule::onCoreRepoOpen);

    updateMostRecentlyUsedMenu();

    registerView<RepoTreeView>(tr("Repository"));
}

void RepositoryModule::deinitialize()
{
    unregisterView<RepoTreeView>();
}

void RepositoryModule::onRepositoryCreate()
{
    CreateRepositoryDlg().exec();
}

void RepositoryModule::onRepositoryOpen()
{
    QWidget* parent = BlueSky::Application::instance()->primaryWindow();
    QFileDialog *fd = new QFileDialog( parent );
    #ifdef Q_OS_MAC
    fd->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    #else
    fd->setFileMode(QFileDialog::Directory);
    #endif

    QString lastUsedDir = Config::self().get( "Repository/lastUsedDir", "#" ).toString();
    if( lastUsedDir != QStringLiteral( "#" ) )
    {
        fd->setDirectory( lastUsedDir );
    }

    fd->setWindowTitle( tr("Open a Git repository") );

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
    QString repoDir = Git::Repository::discover(r, fd->selectedFiles().first());
    if( repoDir.isEmpty() )
    {
        return;
    }

    RM::RepoMan::instance().open(repoDir);
    Config::self().set("Repository/lastUsedDir", repoDir);
}

void RepositoryModule::onRepositoryClone()
{
    CloneRepositoryDlg().exec();
}

void RepositoryModule::onRecentRepositoryOpen( const QVariant& path )
{
    QString repoPath = path.toString();
    RM::RepoMan::instance().open( repoPath );
}

void RepositoryModule::onCoreRepoOpen(const RM::Frontend::Repo& repo)
{
    if( repo.isSubModule() )
    {
        return;
    }

    QString path = repo.path();
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
