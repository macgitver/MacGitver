/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Nils Fenner <nils@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libBlueSky/Dialog.hpp"

namespace Ui
{
    class ExpandableDlg;
}

class MGV_CORE_API ExpandableDlg : public BlueSky::Dialog
{
    Q_OBJECT

public:
    ExpandableDlg();

protected:
    void setDialogWidgets(QWidget* mainWdgt, QWidget* optsWdgt);

protected:
    void setAcceptable(bool valid);

private slots:
    void on_btnShowOpts_toggled(bool checked);

private:
    Ui::ExpandableDlg*  ui;

private:
    QWidget*    mOptsWdgt = nullptr;
};
