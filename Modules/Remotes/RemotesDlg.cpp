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

#include "RemotesDlg.h"

#include "libMacGitverCore/RepoMan/Remote.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include <QMessageBox>

RemotesOptionsWdgt::RemotesOptionsWdgt()
{
    setupUi(this);
}


RemotesWdgt::RemotesWdgt()
{
    setupUi(this);
}


RemotesDlg::RemotesDlg(RM::Repo* repo)
{
    setDialogWidgets(mRemotesWdgt, mRemotesOptsWdgt);

    connect(mRemotesWdgt->btnAddRemote, &QAbstractButton::clicked,
            this,                       &RemotesDlg::onAddRemote);
    connect(mRemotesWdgt->btnDeleteRemote,  &QAbstractButton::clicked,
            this,                           &RemotesDlg::onDeleteRemote);
    connect(mRemotesWdgt->txtUrl,   &QLineEdit::textChanged,
            this,                   &RemotesDlg::onUrlChanged);

    connect( mRemotesOptsWdgt->txtPushUrl,  &QLineEdit::textChanged,
             this,                          &RemotesDlg::checkValid );

    if (!repo) {
        // TODO: this should never happen and we should assert out here.
        setEnabled(false);
        setWindowTitle( tr("Error: Needs a repository context!") );
        return;
    }

    setWindowTitle( tr("Manage the Remotes in \"%1\"").arg(repo->displayName()) );

    mRemotesWdgt->txtRemotes->installEventFilter(this);
}

void RemotesDlg::updateValues(const QString& remoteAlias)
{    
    // TODO: implement dialog update
    //RM::Remote* remote = mRepoContext->remotes(remoteName);
    //txtUrl->setText( remote ? remote->url() : QString() );
    //txtPushUrl->setText( remote ? remote->pushUrl() : QString() );
    //treeRefSpecs->model()->setRemote(remote);
}

/**
 * @internal
 * @brief       Clear the fields for the current selected Remote.
 */
void RemotesDlg::clear()
{
    mRemotesWdgt->txtUrl->clear();

    mRemotesOptsWdgt->txtEditRefSpec->clear();
    mRemotesOptsWdgt->txtPushUrl->clear();
    mRemotesOptsWdgt->treeRefSpecs->setModel(nullptr);
}

bool RemotesDlg::modeCanEnter(RemotesDlg::EditMode mode) const
{
    if (mode == EditMode::Rename) {
        return mRemotesWdgt->txtRemotes->currentIndex() > -1;
    }

    return true;
}

void RemotesDlg::modeEnter(EditMode mode)
{
    Q_ASSERT(mMode == EditMode::Edit);
    if (!modeCanEnter(mode)) {
        return;
    }

    mMode = mode;

    mRemotesWdgt->btnAddRemote->setEnabled(false);
    mRemotesWdgt->btnDeleteRemote->setEnabled(false);
    mRemotesWdgt->txtRemotes->setEditable(true);

    switch (mMode) {
    case EditMode::Create:

        mRemotesWdgt->txtRemotes->lineEdit()->setPlaceholderText(tr("Enter a name ..."));
        //txtRemotes->lineEdit()->clear();
        mRemotesWdgt->txtUrl->setPlaceholderText(tr("Enter a URL for the new Remote ..."));
        clear();

        break;

    case EditMode::Rename:
        mRemotesWdgt->txtRemotes->lineEdit()->setText(mRemotesWdgt->txtRemotes->currentText());
        mRemotesWdgt->txtRemotes->lineEdit()->selectAll();

        break;

    case EditMode::Edit:
    default:
        break;
    }

    mRemotesWdgt->txtRemotes->setFocus();
}

void RemotesDlg::modeLeave(bool accept)
{
    if (accept) {
        switch (mMode) {
        case EditMode::Create:
        {
            QString remoteAlias = mRemotesWdgt->txtRemotes->lineEdit()->text();
            // TODO: implementation to create an RM::Remote
            qCritical("Not implemented yet: Create a Remote via RepoMan.");
            break;
        }

        case EditMode::Rename:
        {
            QString oldRemoteAlias = mRemotesWdgt->txtRemotes->currentText();
            QString newRemoteAlias = mRemotesWdgt->txtRemotes->lineEdit()->text();
            // TODO: implementation to rename an RM::Remote
            qCritical("Not implemented yet: Rename a Remote via RepoMan.");
            break;
        }

        case EditMode::Edit:
        default:
            break;
        }
    }

    mRemotesWdgt->txtUrl->setPlaceholderText(QString());
    mRemotesWdgt->btnAddRemote->setEnabled(true);
    mRemotesWdgt->btnDeleteRemote->setEnabled(true);
    mRemotesWdgt->txtRemotes->setEditable(false);
    mRemotesWdgt->txtRemotes->update();

    // switch to default mode
    mMode = EditMode::Edit;
}

void RemotesDlg::checkValid()
{
    bool okay = !mRemotesWdgt->txtRemotes->currentText().isEmpty();
    okay &= !mRemotesWdgt->txtUrl->text().isEmpty();

    setAcceptable(okay);
}

void RemotesDlg::accept()
{
    if (mMode == EditMode::Edit) {
        QDialog::accept();
    }
    else {
        modeLeave(true);
    }
}

void RemotesDlg::onAddRemote()
{
    modeEnter(EditMode::Create);
}

void RemotesDlg::onDeleteRemote()
{
    int r = QMessageBox::question(this, tr("Delete selected Remote?"),
                                  tr("Delete the Remote \"%1\" from the "
                                     "repository \"%2\"?")
                                  .arg(mRemotesWdgt->txtRemotes->currentText())
                                  .arg(mRepoContext->displayName()));
    if (r == QMessageBox::Yes) {
        // TODO: implement deletion of selected Remote
    }
}

void RemotesDlg::onCurrentRemoteChanged(const QString &alias)
{
    mRemotesWdgt->btnDeleteRemote->setEnabled(mRemotesWdgt->txtRemotes->currentIndex() > -1);
    updateValues(alias);
}

void RemotesDlg::onUrlChanged(const QString& newUrl)
{
    if( !mRemotesOptsWdgt->chkPushUrl->isChecked() ) {
        mRemotesOptsWdgt->txtPushUrl->setText( newUrl );
    }
    checkValid();
}

void RemotesDlg::reject()
{
    if (mMode != EditMode::Edit) {
        modeLeave(false);
    }
    else {
        QDialog::reject();
    }
}

/**
 * @internal
 *
 * @brief       Handles the double-click for the remotes combo box.
 *
 * @param       o   the event sender
 *
 * @param       e   the event
 *
 * @return
 */
bool RemotesDlg::eventFilter(QObject* o, QEvent* e)
{
    if (Q_LIKELY(o == mRemotesWdgt->txtRemotes)) {
        if (e->type() == QEvent::MouseButtonDblClick) {
            modeEnter(EditMode::Rename);
            return true;
        }
    }

    return QObject::eventFilter(o, e);
}
