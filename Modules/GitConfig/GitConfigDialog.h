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

#ifndef MGV_GITCONFIGDIALOG_H
#define MGV_GITCONFIGDIALOG_H

#include "libBlueSky/Dialog.hpp"

#include "libGitWrap/Repository.hpp"

#include "ui_GitConfigDialog.h"

class GitConfigDialog
        : public BlueSky::Dialog
        , private Ui::GitConfigDialog
{
    Q_OBJECT
public:
    GitConfigDialog( Git::Repository& repo );

private:
    Git::Repository mRepo;
};

#endif
