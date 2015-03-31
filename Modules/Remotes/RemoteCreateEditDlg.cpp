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

#include "RemoteCreateEditDlg.h"

#include "libMacGitverCore/RepoMan/Remote.hpp"
#include "libMacGitverCore/RepoMan/Repo.hpp"

#include <QMessageBox>


RemoteCreateEditDlg::RemoteCreateEditDlg(RM::Repo* repo)
{
    setupUi(this);
    init();

    if (!repo) {
        // TODO: this should never happen and we should assert out here.
        setEnabled(false);
        setWindowTitle( tr("Error: Needs a repository context!") );
        return;
    }

    setWindowTitle( tr("Manage the Remotes in \"%1\"").arg(repo->displayName()) );

    txtRemotes->installEventFilter(this);
}

void RemoteCreateEditDlg::init()
{
    connect( txtPushUrl, &QLineEdit::textChanged,
             this, &RemoteCreateEditDlg::checkValid );
}

void RemoteCreateEditDlg::updateValues(const QString& remoteAlias)
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
void RemoteCreateEditDlg::clear()
{
    txtEditRefSpec->clear();
    txtUrl->clear();
    txtPushUrl->clear();
    treeRefSpecs->setModel(nullptr);
}

bool RemoteCreateEditDlg::modeCanEnter(RemoteCreateEditDlg::EditMode mode) const
{
    if (mode == EditMode::Rename) {
        return txtRemotes->currentIndex() > -1;
    }

    return true;
}

void RemoteCreateEditDlg::modeEnter(EditMode mode)
{
    Q_ASSERT(mMode == EditMode::Edit);
    if (!modeCanEnter(mode)) {
        return;
    }

    mMode = mode;

    btnAddRemote->setEnabled(false);
    btnDeleteRemote->setEnabled(false);
    txtRemotes->setEditable(true);

    switch (mMode) {
    case EditMode::Create:

        txtRemotes->lineEdit()->setPlaceholderText(tr("Enter a name ..."));
        //txtRemotes->lineEdit()->clear();
        txtUrl->setPlaceholderText(tr("Enter a URL for the new Remote ..."));
        clear();

        break;

    case EditMode::Rename:
        txtRemotes->lineEdit()->setText(txtRemotes->currentText());
        txtRemotes->lineEdit()->selectAll();

        break;

    case EditMode::Edit:
    default:
        break;
    }

    txtRemotes->setFocus();
}

void RemoteCreateEditDlg::modeLeave(bool accept)
{
    if (accept) {
        switch (mMode) {
        case EditMode::Create:
        {
            QString remoteAlias = txtRemotes->lineEdit()->text();
            // TODO: implementation to create an RM::Remote
            qCritical("Not implemented yet: Create a Remote via RepoMan.");
            break;
        }

        case EditMode::Rename:
        {
            QString oldRemoteAlias = txtRemotes->currentText();
            QString newRemoteAlias = txtRemotes->lineEdit()->text();
            // TODO: implementation to rename an RM::Remote
            qCritical("Not implemented yet: Rename a Remote via RepoMan.");
            break;
        }

        case EditMode::Edit:
        default:
            break;
        }
    }

    txtUrl->setPlaceholderText(QString());
    btnAddRemote->setEnabled(true);
    btnDeleteRemote->setEnabled(true);
    txtRemotes->setEditable(false);
    txtRemotes->update();

    // switch to default mode
    mMode = EditMode::Edit;
}

void RemoteCreateEditDlg::checkValid()
{
    bool okay = !txtRemotes->currentText().isEmpty();
    okay &= !txtUrl->text().isEmpty();

    buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );
}

void RemoteCreateEditDlg::accept()
{
    if (mMode == EditMode::Edit) {
        QDialog::accept();
    }
    else {
        modeLeave(true);
    }
}

void RemoteCreateEditDlg::on_btnAddRemote_clicked()
{
    modeEnter(EditMode::Create);
}

void RemoteCreateEditDlg::on_btnDeleteRemote_clicked()
{
    int r = QMessageBox::question(this, tr("Delete selected Remote?"),
                                  tr("Delete the Remote \"%1\" from the repository \"%2\"?")
                                  .arg(txtRemotes->currentText()).arg(mRepoContext->displayName()));
    if (r == QMessageBox::Yes) {
        // TODO: implement deletion of selected Remote
    }
}

void RemoteCreateEditDlg::on_txtRemotes_currentIndexChanged(const QString &remoteAlias)
{
    btnDeleteRemote->setEnabled( txtRemotes->currentIndex() > -1 );
    updateValues(remoteAlias);
}

void RemoteCreateEditDlg::on_txtUrl_textChanged( const QString& newUrl )
{
    if( !chkPushUrl->isChecked() ) {
        txtPushUrl->setText( newUrl );
    }
    checkValid();
}

void RemoteCreateEditDlg::reject()
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
bool RemoteCreateEditDlg::eventFilter(QObject* o, QEvent* e)
{
    if (Q_LIKELY(o == txtRemotes)) {
        if (e->type() == QEvent::MouseButtonDblClick) {
            modeEnter(EditMode::Rename);
            return true;
        }
    }

    return QObject::eventFilter(o, e);
}
