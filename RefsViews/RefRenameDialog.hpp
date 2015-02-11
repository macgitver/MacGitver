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

#include "libBlueSky/Dialog.hpp"

#include "libGitWrap/Result.hpp"

#include "ui_RefRenameDialog.h"

class RefBranch;

class RefRenameDialog
        : public BlueSky::Dialog
        , private Ui::RefRenameDialog
{
    Q_OBJECT
public:
    RefRenameDialog(RefBranch* ref);

    const Git::Result &gitResult() const;

private slots:
    void accept();

private:
    RefBranch*              mRefInfo;
    Git::Result             mGitResult;
};

#endif // REF_RENAME_DIALOG_HPP
