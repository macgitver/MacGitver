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

#include "GitWrap/ObjectId.h"

#include "SubmodulesCreateEditDlg.h"

SubmodulesCreateEditDlg::SubmodulesCreateEditDlg()
	: mSubmodule()
{
	setupUi( this );
	setupComboboxes();
	init();
	setWindowTitle( trUtf8( "Create a submodule" ) );
}

SubmodulesCreateEditDlg::SubmodulesCreateEditDlg( Git::Submodule module )
	: mSubmodule( module )
{
	setupUi( this );
	setupComboboxes();

	mSubmodule = module;

	txtName->setText( module.name() );
	txtPath->setText( module.path() );

	txtSHA1->setText( module.currentSHA1().toString() );
	txtUrl->setText( module.url() );

	if( module.name() != module.path() )
	{
		chkHasPath->setChecked( true );
	}
	chkHasPath->setEnabled( false );
	txtPath->setEnabled( false );
	txtName->setEnabled( false );

	chkFetchRecursive->setChecked( module.fetchRecursive() );

	init();
	setWindowTitle( trUtf8( "Edit submodule" ) );
}

void SubmodulesCreateEditDlg::setupComboboxes()
{
	cboIgnoreStrategy->addItem( trUtf8( "None (Default)" ), Git::Submodule::None );
	cboIgnoreStrategy->addItem( trUtf8( "Dirty" ), Git::Submodule::Dirty );
	cboIgnoreStrategy->addItem( trUtf8( "Untracked" ), Git::Submodule::Untracked );
	cboIgnoreStrategy->addItem( trUtf8( "All" ), Git::Submodule::All );

	cboUpdateStrategy->addItem( trUtf8( "Checkout (Default)" ), Git::Submodule::Checkout );
	cboUpdateStrategy->addItem( trUtf8( "Rebase" ), Git::Submodule::Rebase );
	cboUpdateStrategy->addItem( trUtf8( "Merge" ), Git::Submodule::Merge );
	cboUpdateStrategy->addItem( trUtf8( "None" ), -1 );
}

void SubmodulesCreateEditDlg::init()
{
	connect(txtName,SIGNAL(textChanged(QString)),
			this,SLOT(onNameChange(QString)) );
	connect(chkHasPath,SIGNAL(toggled(bool)),
			this, SLOT(onHasPathChange(bool)) );
}

void SubmodulesCreateEditDlg::onNameChange( const QString& name )
{
	if( !chkHasPath->isChecked() )
	{
		txtPath->setText( name );
	}
}

void SubmodulesCreateEditDlg::onHasPathChange( bool newValue )
{
	if( !newValue )
	{
		txtPath->setText( txtName->text() );
	}
}
