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

#include "GitWrap/Git.h"

#include "CreateRepositoryDlg.h"

#include "MacGitver/MacGitver.h"

CreateRepositoryDlg::CreateRepositoryDlg()
{
	setupUi( this );

	connect( btnSelectPath, SIGNAL(clicked()), SLOT(onBrowse()) );
	connect( txtPath, SIGNAL(textChanged(QString)), SLOT(checkValid()) );

	checkValid();
}

void CreateRepositoryDlg::onBrowse()
{
	QString fn = txtPath->text();
	fn = QFileDialog::getExistingDirectory( this, trUtf8( "Browse" ), fn );
	if( fn.isEmpty() )
		return;

	txtPath->setText( fn );
}

void CreateRepositoryDlg::checkValid()
{
	bool okay = !txtPath->text().isEmpty();

	QDir wanted( txtPath->text() );
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

void CreateRepositoryDlg::accept()
{
	QString fn = QDir::toNativeSeparators( txtPath->text() );
	bool makeBare = chkMakeBare->isChecked() && chkMakeBare->isEnabled();
	Git::Repository repo = Git::Repository::create( fn, makeBare );

	if( !repo.isValid() )
	{
		QMessageBox::critical( this,
							   trUtf8( "Error" ),
							   trUtf8( "Failed to create the repository." ) );
		return;
	}

	QDialog::accept();
	MacGitver::self().setRepository( repo );
}
