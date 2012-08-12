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

#include "GitConfigDialog.h"

GitConfigDialog::GitConfigDialog( QWidget* parent, Git::Repository& repo )
	: QDialog( parent )
	, mRepo( repo )
{
	setupUi( this );

	QStringList cfgs;

	QString fnGlobal = Git::Config::globalFilePath();
	QString fnUser = Git::Config::userFilePath();
	QString fnRepo;

	cfgSystem->setConfig( fnGlobal );
	cfgUser->setConfig( fnUser );

	if( mRepo.isValid() )
	{
		fnRepo = mRepo.gitPath() + QLatin1String( "/config" );
		cfgRepository->setConfig( fnRepo );
		cfgs << fnRepo;
	}
	else
	{
		tabWidget->setTabEnabled( tabWidget->indexOf( cfgRepository ), false );
	}

	tabWidget->setCurrentWidget( cfgCombined );

	cfgs << fnUser << fnGlobal;

	cfgCombined->setConfigs( cfgs );
}
