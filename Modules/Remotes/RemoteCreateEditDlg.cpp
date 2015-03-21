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

#include "RemoteCreateEditDlg.h"

RemoteCreateEditDlg::RemoteCreateEditDlg()
{
    setupUi( this );
    setWindowTitle( trUtf8( "Create remote" ) );
    init();
}

RemoteCreateEditDlg::RemoteCreateEditDlg( Git::Remote remote )
{
    setupUi( this );
    setWindowTitle( trUtf8( "Edit remote" ) );
    init();

    mRemote = remote;
    txtName->setText( mRemote.name() );
    txtUrl->setText( mRemote.url() );
}

void RemoteCreateEditDlg::init()
{
    connect( txtName, SIGNAL(textChanged(QString)), this, SLOT(onNameChanged(QString)) );
    connect( txtUrl, SIGNAL(textChanged(QString)), this, SLOT(onUrlChanged(QString)) );
    connect( txtPushUrl, SIGNAL(textChanged(QString)), this, SLOT(checkValid()) );
}

void RemoteCreateEditDlg::onNameChanged( const QString& newName )
{
    if( !chkRefSpecs->isChecked() )
    {
        // ...
    }
    checkValid();
}

void RemoteCreateEditDlg::onUrlChanged( const QString& newUrl )
{
    if( !chkPushUrl->isChecked() )
    {
        txtPushUrl->setText( newUrl );
    }
    checkValid();
}

void RemoteCreateEditDlg::checkValid()
{
    bool okay =
            !txtName->text().isEmpty() &&
            !txtUrl->text().isEmpty() &&
            ( !chkPushUrl->isChecked() ||
              ( chkPushUrl->isChecked() && !txtPushUrl->text().isEmpty() ) );

    buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );
}

void RemoteCreateEditDlg::accept()
{
}
