
#include "ProgressDlg.hpp"
#include "ui_ProgressDlg.h"
#include "ui_ProgressWdgt.h"

#include <QCloseEvent>
#include <QPushButton>
#include <QStringBuilder>

namespace Private
{

    class ProgressWdgt : public QWidget, public Ui::ProgressWdgt
    {
    public:
        enum Status { Running = 0, Stopped };

        typedef QMap< QString, QPointer<ProgressWdgt> > Steps;

    public:
        ProgressWdgt(const QString& description)
            : mStatus(Running)
            , mPercentage(0.)
        {
            setupUi(this);
            progressBar->setMinimum(0);
            progressBar->setMaximum(100);
            txtHeader->setText( description );
        }

    public:
        Steps   mSteps;
        Status  mStatus;

        qreal   mPercentage;
    };

}


ProgressDlg::ProgressDlg()
    : BlueSky::Dialog()
    , ui( new Ui::ProgressDlg )
    , mDone( false )
{
    ui->setupUi( this );

    QPushButton* close = ui->buttonBox->button(QDialogButtonBox::Close);
    close->setEnabled( false );
    connect(close, &QPushButton::clicked, this, &ProgressDlg::close);

    connect(&mUpdater, &QTimer::timeout, this, &ProgressDlg::updateActivities);
    mUpdater.start(250);
}

ProgressDlg::~ProgressDlg()
{
    delete ui;
}

int ProgressDlg::updateInterval() const
{
    return mUpdater.interval();
}

void ProgressDlg::setUpdateInterval(int msec)
{
    if (mUpdater.isActive()) {
        // restart timer with the new interval
        mUpdater.start(msec);
    }
    else {
        mUpdater.setInterval(msec);
    }
}

void ProgressDlg::addActivity(const QString& description, QObject* activity,
                              const StepInfo::List& steps)
{
    Q_ASSERT(activity);

    Private::ProgressWdgt* a = new Private::ProgressWdgt(description);
    mActivities[activity] = a;

    QTreeWidgetItem* activityItem(new QTreeWidgetItem);
    ui->treeProgress->addTopLevelItem(activityItem);
    ui->treeProgress->setItemWidget(activityItem, 0, a);

    foreach (const StepInfo& sir, steps) {
        Private::ProgressWdgt* s = new Private::ProgressWdgt(sir.desc);
        a->mSteps[sir.name] = s;

        QTreeWidgetItem* stepItem(new QTreeWidgetItem);
        activityItem->addChild(stepItem);
        ui->treeProgress->setItemWidget(stepItem, 0, s);
    }

    a->txtStatusInfo->setText(tr("Running..."));
}

void ProgressDlg::setStatusInfo(QObject* activity, const QString& step,
                                const QString& text)
{
    Private::ProgressWdgt* s = findStep(activity, step);
    Q_ASSERT(s);

    s->txtStatusInfo->setText(text);
}

void ProgressDlg::setPercentage(QObject* activity, const QString& step,
                                qreal percent)
{
    Private::ProgressWdgt* s = findStep(activity, step);
    Q_ASSERT(s);

    s->mPercentage = qMin( qMax(percent, 0.), 1. ) * 100.;
}

void ProgressDlg::closeEvent( QCloseEvent* ev )
{
    if( !mDone )
    {
        ev->ignore();
        return;
    }

    QDialog::closeEvent( ev );
}

void ProgressDlg::finished(QObject* activity)
{
    Private::ProgressWdgt* a = mActivities[activity];
    Q_ASSERT(a);

    a->mStatus = Private::ProgressWdgt::Stopped;
    a->txtStatusInfo->setText(tr("Finished!"));

    bool done = true;
    foreach (Private::ProgressWdgt* a, mActivities) {
        done &= (a->mStatus == Private::ProgressWdgt::Stopped);
        if (!done) {
            break;
        }
    }

    if (done) {
        mDone = true;
        ui->buttonBox->button( QDialogButtonBox::Close )->setEnabled( true );
    }
}

void ProgressDlg::finished(QObject* activity, const QString& step)
{
    Q_ASSERT(activity && !step.isEmpty());

    Private::ProgressWdgt* a = mActivities[activity];
    a->mSteps[step]->mStatus = Private::ProgressWdgt::Stopped;
}

void ProgressDlg::updateActivities()
{
    foreach(Private::ProgressWdgt* a, mActivities) {
        a->mPercentage = 0;
        foreach (Private::ProgressWdgt* s, a->mSteps) {
            s->progressBar->setValue(qRound(s->mPercentage));
            qreal stepPercent = s->mPercentage / a->mSteps.size();
            a->mPercentage += qMin( qMax(stepPercent, 0.), 100.);
        }

        a->progressBar->setValue(qRound(a->mPercentage));
    }
}

Private::ProgressWdgt* ProgressDlg::findStep(QObject* activity, const QString& step) const
{
    Private::ProgressWdgt* a = mActivities[activity];
    return step.isEmpty() || !a ? nullptr : a->mSteps[step];
}

void ProgressDlg::setCanClose()
{
    const bool wasDone = mDone;
    mDone = isDone();

    if (mDone != wasDone) {
        ui->buttonBox->button( QDialogButtonBox::Close )->setEnabled( mDone );
    }
}
