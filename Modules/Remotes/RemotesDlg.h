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

#pragma once

#include "ui_RemotesDlg.h"

namespace RM
{
    class Repo;
}

class RemotesDlg : public QDialog, private Ui::RemotesDlg
{
    Q_OBJECT

public:
    enum EditMode
    {
        Edit = 0,
        Create,
        Rename
    };

public:
    RemotesDlg(RM::Repo* repo);

private:
    void init();

private:
    // virtual slots
    void accept();
    void reject();

private:
    bool eventFilter(QObject* o, QEvent* e);

    void updateValues(const QString &remoteAlias);
    void clear();

    bool modeCanEnter(EditMode mode) const;
    void modeEnter(EditMode mode);
    void modeLeave(bool accept);

private slots:
    void checkValid();

private slots:
    // auto-connected ui slots
    void on_btnAddRemote_clicked();
    void on_btnDeleteRemote_clicked();
    void on_txtRemotes_currentIndexChanged(const QString &remoteAlias);
    void on_txtUrl_textChanged(const QString& newUrl);

private:
    RM::Repo*       mRepoContext = nullptr;
    EditMode        mMode = EditMode::Edit;
};
