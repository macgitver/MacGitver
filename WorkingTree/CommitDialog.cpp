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

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include "libGitWrap/Operations/CommitOperation.hpp"
#include "libGitWrap/Commit.hpp"
#include "libGitWrap/Index.hpp"
#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Result.hpp"
#include "libGitWrap/Tree.hpp"

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
    connect(ui->btnAmend, SIGNAL(toggled(bool)), this, SLOT(onAmend(bool)));
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

    RM::Repo *repo = MacGitver::repoMan().activeRepository();
    if (!repo)
    {
        QMessageBox::warning( this, trUtf8("Failed to commit"),
                              trUtf8("Cannot amend a commit without an active repository.") );
        return;
    }

    Git::Repository gitRepo = repo->gitRepo();

    QScopedPointer<Git::CommitOperation> op( gitRepo.commitOperation( r, ui->textCommitMessage->toPlainText() ) );
    if ( op && r )
    {
        op->execute();
        r = op->result();
    }

    if ( !r )
    {
        QMessageBox::information( this, trUtf8("Failed to commit"),
                                  trUtf8("Failed to commit. Git message:\n%1").arg(r.errorText()));
    }
}

void CommitDialog::onAmend(bool enabled)
{
    Git::Result r;

    RM::Repo *repo = MacGitver::repoMan().activeRepository();
    if (!repo)
    {
        QMessageBox::warning( this, trUtf8("Failed to amend commit"),
                              trUtf8("Cannot amend a commit without an active repository.") );
    }

    Git::Commit c =  repo->gitRepo().HEAD(r).peeled(r, Git::otCommit).asCommit();

    if (!r)
    {
        QMessageBox::warning( this, trUtf8("Failed to amend commit"),
                              trUtf8("Amend failed. Git message:\n%1").arg(r.errorText()) );
    }

    if (!c.isValid())
    {
        QMessageBox::information( this, trUtf8("Failed to amend commit"),
                                  trUtf8("No commit found to amend.") );
    }

    // TODO: implement logic to amend the last commit in the git repository
}
