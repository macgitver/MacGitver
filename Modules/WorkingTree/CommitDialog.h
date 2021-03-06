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

#ifndef MGV_COMMIT_DIALOG_H
#define MGV_COMMIT_DIALOG_H

#include <QWidget>

namespace Ui
{
    class CommitDialog;
}

class CommitDialog : public QWidget
{
    Q_OBJECT
public:
    CommitDialog(QWidget *parent = 0);
    ~CommitDialog();

    QString message() const;
    void setCommitMessage(const QString &message);

signals:
    void amendHeadCommit(bool enabled);

private slots:
    void onCommit();
    void onAmend(bool enabled);

private:
    Ui::CommitDialog *  ui;
};

#endif
