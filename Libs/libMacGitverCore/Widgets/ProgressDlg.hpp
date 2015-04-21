
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
    void setAction( const QString& action, const QStringList& open,
                    const QStringList& current, const QStringList& done );
    void setCurrent(QObject* current);

private slots:
    void transportProgress( quint32 totalObjects, quint32 indexedObjects,
                            quint32 receivedObjects, quint64 receivedBytes );
    void remoteMessage( const QString& msg );

public:
    void setDone();
    void beginStep( const QString& step );
    void finalizeStep();

protected:
    void closeEvent( QCloseEvent* ev );

private:
    Ui::ProgressDlg*    ui;

private:
    bool            mDone;
    QString         mBaseLog;
    QObject*        mCurrent;
    QString         mRawRemoteMessage;
};
