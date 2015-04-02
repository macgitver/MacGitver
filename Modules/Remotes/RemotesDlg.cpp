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


RemotesDlg::RemotesDlg(RM::Repo* repo)
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

void RemotesDlg::init()
{
    connect( txtPushUrl, &QLineEdit::textChanged,
             this, &RemotesDlg::checkValid );
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
    txtEditRefSpec->clear();
    txtUrl->clear();
    txtPushUrl->clear();
    treeRefSpecs->setModel(nullptr);
}

bool RemotesDlg::modeCanEnter(RemotesDlg::EditMode mode) const
{
    if (mode == EditMode::Rename) {
        return txtRemotes->currentIndex() > -1;
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

void RemotesDlg::modeLeave(bool accept)
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

void RemotesDlg::checkValid()
{
    bool okay = !txtRemotes->currentText().isEmpty();
    okay &= !txtUrl->text().isEmpty();

    buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );
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

void RemotesDlg::on_btnAddRemote_clicked()
{
    modeEnter(EditMode::Create);
}

void RemotesDlg::on_btnDeleteRemote_clicked()
{
    int r = QMessageBox::question(this, tr("Delete selected Remote?"),
                                  tr("Delete the Remote \"%1\" from the repository \"%2\"?")
                                  .arg(txtRemotes->currentText()).arg(mRepoContext->displayName()));
    if (r == QMessageBox::Yes) {
        // TODO: implement deletion of selected Remote
    }
}

void RemotesDlg::on_txtRemotes_currentIndexChanged(const QString &remoteAlias)
{
    btnDeleteRemote->setEnabled( txtRemotes->currentIndex() > -1 );
    updateValues(remoteAlias);
}

void RemotesDlg::on_txtUrl_textChanged( const QString& newUrl )
{
    if( !chkPushUrl->isChecked() ) {
        txtPushUrl->setText( newUrl );
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
    if (Q_LIKELY(o == txtRemotes)) {
        if (e->type() == QEvent::MouseButtonDblClick) {
            modeEnter(EditMode::Rename);
            return true;
        }
    }

    return QObject::eventFilter(o, e);
}
