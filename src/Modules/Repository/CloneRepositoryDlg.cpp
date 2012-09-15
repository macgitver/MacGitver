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

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

#include "Config/Config.h"

#include "GitWrap/GitWrap.h"
#include "GitWrap/BackgroundClone.h"

#include "CloneRepositoryDlg.h"

#include "MacGitver/MacGitver.h"

CloneRepositoryDlg::CloneRepositoryDlg()
{
	setupUi( this );

	connect( btnBrowse, SIGNAL(clicked()), SLOT(onBrowse()) );
	connect( txtPath, SIGNAL(textChanged(QString)), SLOT(checkValid()) );
	connect( chkCheckout, SIGNAL(toggled(bool)), this, SLOT(onCheckout(bool)) );
	connect( chkInitSubmodules, SIGNAL(toggled(bool)), this, SLOT(onInitSubmodules(bool)) );
	connect( txtCheckoutBranch, SIGNAL(textChanged(QString)), this, SLOT(onCheckoutBranch(QString)) );

	checkValid();
}

void CloneRepositoryDlg::onBrowse()
{
	QString fn = txtPath->text();
	if( fn.isEmpty() )
	{
		fn = Config::self().get( "Repository/lastUsedDir", QDir::homePath() ).toString();
	}

	QFileDialog* fd = new QFileDialog( this, trUtf8( "Select repository location" ) );

	#ifdef Q_OS_MAC
	fd->setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );
	#else
	fd->setFileMode( QFileDialog::Directory );
	#endif

	fd->setDirectory( fn );
	fd->setAcceptMode( QFileDialog::AcceptSave );
	fd->open( this, SLOT(onBrowseHelper(QString)) );
}

void CloneRepositoryDlg::onBrowseHelper( const QString& directory )
{
	if( directory.isEmpty() )
	{
		return;
	}

	Config::self().set( "Repository/lastUsedDir", directory );
	txtPath->setText( directory );
}

void CloneRepositoryDlg::onCheckout( bool value )
{
	txtCheckoutBranch->setEnabled( value );
	chkInitSubmodules->setEnabled( value );

	if( !value )
	{
		chkInitSubmodules->setChecked( false );
		txtCheckoutBranch->setText( QString() );
	}
}

void CloneRepositoryDlg::onInitSubmodules( bool value )
{
	chkSubmodulesRecursive->setEnabled( value );
	if( !value )
	{
		chkSubmodulesRecursive->setChecked( false );
	}
}

void CloneRepositoryDlg::onCheckoutBranch( const QString& branch )
{
	if( branch.isEmpty() )
	{
		chkFetchOne->setDisabled( true );
		chkFetchOne->setChecked( false );
	}
	else
	{
		chkFetchOne->setEnabled( true );
	}
}

void CloneRepositoryDlg::checkValid()
{
	bool okay =
			!txtPath->text().isEmpty() &&
			!txtUrl->text().isEmpty() &&
			!txtRemoteName->text().isEmpty();

	QDir wanted( QDir::toNativeSeparators( txtPath->text() ) );
	if( wanted.exists() )
	{
		QStringList sl = wanted.entryList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
		if( sl.count() > 0 )
		{
			okay = false;
		}
	}

	buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );
}

void CloneRepositoryDlg::accept()
{
	Git::BackgroundClone* clone = new Git::BackgroundClone;
	clone->setFrom( txtUrl->text(), txtPath->text() );
	clone->setType(
				chkCloneBare->isChecked(),
				chkCloneMirror->isChecked(),
				chkCheckout->isChecked() );
	clone->setRemoteName( txtRemoteName->text() );
	clone->setSubmodule(
				chkInitSubmodules->isChecked(),
				chkSubmodulesRecursive->isChecked() );

	Git::Repository repo = clone->repository();

	Git::BackgroundThead* thread = new Git::BackgroundThead();
	thread->queue( clone );
	thread->start();
	thread->wait();
}
