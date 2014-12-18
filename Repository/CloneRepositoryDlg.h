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

#ifndef CLONE_REPOSITORY_DLG_H
#define CLONE_REPOSITORY_DLG_H

#include "ui_CloneRepositoryDlg.h"

#include <QPointer>

class ProgressDlg;
class CloneOptionsWdgt;

class CloneRepositoryDlg : public QDialog, Ui::CloneRepositoryDlg
{
    Q_OBJECT
public:
    CloneRepositoryDlg();

protected:
    void accept();

private slots:
    void onBrowse();
    void onBrowseHelper( const QString& directory );
    void checkValid();

    void doneIndexing();
    void doneCheckout();
    void doneDownload();
    void beginDownloading();
    void rootCloneFinished();

    void on_btnCloneopts_toggled(bool checked);

private:
    enum State { Unused, Open, Done, Current };
    enum Tasks { Prepare, Download, Index, Checkout };

private:
    void updateAction();

private:
    QPointer<CloneOptionsWdgt>     mCloneOpts;

    ProgressDlg*                mProgress;
    QString                     mAction;
    QHash< Tasks, State >       mStates;

};

#endif
