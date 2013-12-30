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

#include "CommitDialog.h"
#include "ui_CommitDialog.h"

#include "libGitWrap/Result.hpp"

#include <QPlainTextEdit>
#include <QLayout>
#include <QMessageBox>


CommitDialog::CommitDialog(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CommitDialog)
{
    ui->setupUi( this );

    setContentsMargins(0, 0, 0, 0);

    connect(ui->btnCommit, SIGNAL(clicked()), this, SLOT(onCommit()));
}

CommitDialog::~CommitDialog()
{
    delete ui;
}

QString CommitDialog::message() const
{
    return ui->textCommitMessage->toPlainText();
}

void CommitDialog::setCommitMessage(const QString &message)
{
    ui->textCommitMessage->setPlainText(message);
}

void CommitDialog::onCommit()
{
    Git::Result r;

    // TODO: implement logic to add a commit to the git repository

    if (!r)
    {
        QMessageBox::information( this, trUtf8("Failed to commit"),
                                  trUtf8("Failed to commit. Git message:\n%1").arg(r.errorText()));
    }
}
