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

#ifndef MGV_SUBMODULES_CREATE_EDIT_DLG_H
#define MGV_SUBMODULES_CREATE_EDIT_DLG_H

#include "libGitWrap/Submodule.h"

#include "ui_SubmodulesCreateEditDlg.h"

class SubmodulesCreateEditDlg : public QDialog, private Ui::SubmodulesCreateEditDlg
{
	Q_OBJECT
public:
	SubmodulesCreateEditDlg();
	SubmodulesCreateEditDlg( Git::Submodule module );

private slots:
	void onNameChange( const QString& name );
	void onHasPathChange( bool newValue );

private:
	void setupComboboxes();
	void init();

private:
	Git::Submodule			mSubmodule;
};

#endif
