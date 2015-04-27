
#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libBlueSky/Dialog.hpp"

#include <QMap>
#include <QPointer>
#include <QTimer>

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
    struct StepInfo
    {
        typedef QVector<StepInfo> List;

        QString name;
        QString desc;
    };

    typedef QMap< QPointer<QObject>, QPointer<Private::ProgressWdgt> > Activities;

public:
    ProgressDlg();
    ~ProgressDlg();

public:
    int updateInterval() const;
    void setUpdateInterval(int msec);

    void addActivity(const QString& description, QObject* activity,
                     const StepInfo::List& steps);

    void setStatusInfo(QObject* activity, const QString& step,
                       const QString& text);
    void setPercentage(QObject* activity, const QString& step, qreal percent);

    void finished(QObject* activity);
    void finished(QObject* activity, const QString& step);

    void setError(QObject* activity, const QString& message);

private slots:
    void updateActivities();

protected:
    void closeEvent( QCloseEvent* ev );

private:
    Private::ProgressWdgt* findStep(QObject* activity, const QString& step) const;

    bool isDone() const;
    void setCanClose();

private:
    Ui::ProgressDlg*    ui;

private:
    bool            mDone;
    QString         mBaseLog;
    QString         mRawRemoteMessage;
    Activities      mActivities;

    QTimer          mUpdater;
};

inline bool ProgressDlg::isDone() const
{
    return mActivities.isEmpty();
}
