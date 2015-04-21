
#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libBlueSky/Dialog.hpp"

#include <QMap>
#include <QPointer>

namespace Private
{
    class ProgressWdgt;
};

namespace Ui
{
    class ProgressDlg;
};

class MGV_CORE_API ProgressDlg : public BlueSky::Dialog
{
    Q_OBJECT

public:
    typedef QMap< QPointer<QObject>, QPointer<Private::ProgressWdgt> > Activities;

public:
    ProgressDlg();
    ~ProgressDlg();

public:



protected:
    void closeEvent( QCloseEvent* ev );

private:
    Ui::ProgressDlg*    ui;

private:
    bool            mDone;
    QString         mBaseLog;
    QString         mRawRemoteMessage;
};
