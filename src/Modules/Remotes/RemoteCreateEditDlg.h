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

#ifndef MGV_REMOTE_CREATE_EDIT_DLG_H
#define MGV_REMOTE_CREATE_EDIT_DLG_H

#include "ui_RemoteCreateEditDlg.h"

#include "libGitWrap/Remote.h"

class RemoteCreateEditDlg : public QDialog, private Ui::RemoteCreateEditDlg
{
    Q_OBJECT
public:
    RemoteCreateEditDlg();
    RemoteCreateEditDlg( Git::Remote remote );

private:
    void init();

private:
    void accept();

private slots:
    void onNameChanged( const QString& newName );
    void onUrlChanged( const QString& newUrl );

    void checkValid();

private:
    Git::Remote mRemote;
};

#endif
