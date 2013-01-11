
#ifndef MODREPO_PROGRESS_DLG_HPP
#define MODREPO_PROGRESS_DLG_HPP

#include "ui_ProgressDlg.h"

class ProgressDlg : public QDialog, private Ui::ProgressDlg
{
    Q_OBJECT
public:
    ProgressDlg( QWidget* parent );

public:
    void setAction( const QString& action, const QStringList& open,
                    const QStringList& current, const QStringList& done );
    void setCurrent( QObject* current );

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
    bool            mDone;
    QString         mBaseLog;
    QObject*        mCurrent;
    QString         mRawRemoteMessage;
};

#endif
