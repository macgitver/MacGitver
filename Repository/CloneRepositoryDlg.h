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

#ifndef CLONE_REPOSITORY_DLG_H
#define CLONE_REPOSITORY_DLG_H

#include "ui_CloneRepositoryDlg.h"

class CloneRepositoryDlg : public QDialog, Ui::CloneRepositoryDlg
{
    Q_OBJECT
public:
    CloneRepositoryDlg();

protected:
    void accept();

private slots:
    void onBrowse();
    void onBrowseHelper( const QString& directory );
    void onCheckout( bool value );
    void onInitSubmodules( bool value );
    void onCheckoutBranch( const QString& branch );
    void checkValid();
};

#endif
