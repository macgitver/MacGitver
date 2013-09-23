/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#ifndef REF_RENAME_DIALOG_HPP
#define REF_RENAME_DIALOG_HPP

#include <QDialog>

#include "libGitWrap/Result.hpp"

namespace Ui
{
    class RefRenameDialog;
}

class RefBranch;

class RefRenameDialog : public QDialog
{
    Q_OBJECT
public:    
    RefRenameDialog( QWidget* parent = 0 );
    ~RefRenameDialog();

    void init( RefBranch *refInfo );
    const Git::Result &gitResult() const;

private slots:
    void accept();

private:
    Ui::RefRenameDialog*    ui;

    RefBranch*              mRefInfo;
    Git::Result             mGitResult;

private:
    void updateValues();
};

#endif // REF_RENAME_DIALOG_HPP
