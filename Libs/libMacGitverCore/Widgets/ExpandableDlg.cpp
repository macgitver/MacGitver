/*
 * MacGitver
 * Copyright (C) 2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Nils Fenner <nils@macgitver.org>
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

#include "ExpandableDlg.hpp"
#include "ui_ExpandableDlg.h"

ExpandableDlg::ExpandableDlg()
    : ui(new Ui::ExpandableDlg)
{
    ui->setupUi(this);
}

void ExpandableDlg::setDialogWidgets(QWidget* mainWdgt, QWidget* optsWdgt)
{
    Q_ASSERT(mainWdgt);
    ui->optsLayout->addWidget(mainWdgt);

    mOptsWdgt = optsWdgt;
    Q_ASSERT(mOptsWdgt && mOptsWdgt->layout());

    // the margin is already defined by ui->optsLayout
    mOptsWdgt->layout()->setMargin(0);

}

void ExpandableDlg::on_btnShowOpts_toggled(bool checked)
{
    Q_ASSERT(mOptsWdgt);

    if ( checked ) {
        ui->optsLayout->addWidget( mOptsWdgt );
        mOptsWdgt->show();
    }
    else {
        ui->optsLayout->removeWidget( mOptsWdgt );
        if ( mOptsWdgt ) {
            mOptsWdgt->hide();
        }

        layout()->activate();
        resize( width(), minimumSizeHint().height() );
    }
}

/**
 * @internal
 *
 * @brief       Set the enabled state of the dialogs OK button.
 *
 * @param       valid   controls the dialogs acceptable state
 */
void ExpandableDlg::setAcceptable(bool valid)
{
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( valid );
}
