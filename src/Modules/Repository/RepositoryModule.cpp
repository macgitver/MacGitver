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

#include <QtPlugin>
#include <QFileDialog>

#include "MacGitver/MacGitver.h"

#include "RepositoryModule.h"
#include "RepositoryCore.h"
#include "CloneRepositoryDlg.h"
#include "CreateRepositoryDlg.h"

RepositoryModule::RepositoryModule()
	: ConfigUser( "Repository" )
{
	mCore = new RepositoryCore;
}

RepositoryModule::~RepositoryModule()
{
	delete mCore;
}

void RepositoryModule::repositoryChanged( Git::Repository newRepository )
{
	mRepo = newRepository;

	Q_ASSERT( mCore );
	mCore->setRepository( newRepository );

	bool isValid = mRepo.isValid();

	actRepositoryClose->setEnabled( isValid );
}

void RepositoryModule::setupConfigPages( IConfigDialog* dialog )
{
}

Module::Types RepositoryModule::providesModuleTypes() const
{
	return Repository;
}

void RepositoryModule::initialize()
{
	setupActions( this );
	acRepositoryMenuAC->mergeInto( "RepositoryMenuMP" );
	acRepositoryToolBarAC->mergeInto( "RepositoryToolBarMP" );
}

void RepositoryModule::deinitialize()
{
}

void RepositoryModule::onRepositoryClose()
{
	if( mRepo.isValid() )
	{
		MacGitver::self().setRepository( Git::Repository() );
	}
}

void RepositoryModule::onRepositoryCreate()
{
	CreateRepositoryDlg().exec();
}

void RepositoryModule::onRepositoryOpen()
{
	QWidget* main = MacGitver::self().mainWindow()->widget();

	QFileDialog *fd = new QFileDialog(main);
#ifdef Q_OS_MAC
	fd->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
#else
	fd->setFileMode(QFileDialog::Directory);
#endif

	QString lastUsedDir = configGet( "lastUsedDir", "#" );
	if( lastUsedDir != QLatin1String( "#" ) )
	{
		fd->setDirectory( lastUsedDir );
	}

	fd->setWindowTitle( trUtf8("Open a Git repository") );

	fd->open(this, SLOT(onRepositoryOpenHelper()));
}

void RepositoryModule::onRepositoryOpenHelper()
{
	QFileDialog *fd = qobject_cast<QFileDialog *>(sender());
	Q_ASSERT(fd != 0);

	if ( fd->selectedFiles().isEmpty() )
		return;

	//! @todo error handling
	Git::Result r;
	QString repoDir = Git::Repository::discover( fd->selectedFiles().first(), false,
												 QStringList(), r );
	if ( repoDir.isEmpty() )
		return;

	//! @todo error handling
	Git::Repository repo = Git::Repository::open( repoDir, r );
	if( !repo.isValid() )
		return;

	// If we successfully loaded the repository at that directory,
	// we store it as "lastUsedDir"
	configSet( "lastUsedDir", repoDir );

	MacGitver::self().setRepository( repo );
}

void RepositoryModule::onRepositoryClone()
{
	CloneRepositoryDlg().exec();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( Repository, RepositoryModule )
#endif
