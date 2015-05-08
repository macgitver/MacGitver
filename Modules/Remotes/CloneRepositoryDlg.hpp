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

#pragma once

#include "libMacGitverCore/Widgets/ExpandableDlg.hpp"

#include "ui_CloneWdgt.h"
#include "ui_CloneOptionsWdgt.h"

#include <QPointer>

class ProgressDlg;

class CloneOptionsWdgt : public QWidget, Ui::CloneOptionsWdgt
{
    Q_OBJECT

    friend class CloneDlg;

public:
    enum CloneMode {
        cmCheckout  = 0,
        cmBare,
        cmMirror
    };

public:
    CloneOptionsWdgt();

private slots:
    void on_txtCloneMode_currentIndexChanged(int index);
    void on_chkInitSubmodules_toggled(bool checked);

private:
    CloneMode       mCloneMode;
};

class CloneWdgt : public QWidget, Ui::CloneWdgt
{
    Q_OBJECT

    friend class CloneDlg;

public:
    CloneWdgt();

private slots:
    void onBrowse();
    void onBrowseHelper( const QString& directory );
};

class CloneDlg : public ExpandableDlg
{
    Q_OBJECT

private:
    enum State { Unused, Open, Done, Current };
    enum Tasks { Prepare, Download, Index, Checkout };

public:
    CloneDlg();

protected:
    void accept();

private slots:
    void checkValid();

    void beginDownloading();
    void rootCloneFinished();

    void onCheckoutProgress(const QString& fileName,
                            quint64 totalFiles,
                            quint64 completedFiles);

    void onTransportProgress(quint32 totalObjects,
                             quint32 indexedObjects,
                             quint32 receivedObjects,
                             quint64 receivedBytes);

private:
    ProgressDlg*                mProgress = nullptr;

    QPointer<CloneWdgt>         mCloneWdgt;
    QPointer<CloneOptionsWdgt>  mCloneOptsWdgt;
};
