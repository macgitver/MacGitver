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
#include "CreateRepositoryDlg.h"

RepositoryModule::RepositoryModule()
{
}

void RepositoryModule::repositoryChanged( Git::Repository newRepository )
{
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
	acRepositoryAC->mergeInto( "RepositoryMenuMP" );
}

void RepositoryModule::deinitialize()
{
}

void RepositoryModule::onRepositoryCreate()
{
	CreateRepositoryDlg().exec();
}

void RepositoryModule::onRepositoryOpen()
{
	QWidget* main = MacGitver::self().mainWindow()->widget();

	QString fn = QFileDialog::getExistingDirectory( main, trUtf8( "Open repository" ) );
	if( fn.isEmpty() )
	{
		return;
	}

	Git::Repository repo = Git::Repository::open( fn );
	if( repo.isValid() )
	{
		MacGitver::self().setRepository( repo );
	}
}

Q_EXPORT_PLUGIN2( Repository, RepositoryModule )
