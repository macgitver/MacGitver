
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
        enum class Result { Undefined = 0, Ok, Error };

        typedef QMap< QString, QPointer<ProgressWdgt> > Steps;

    public:
        ProgressWdgt(const QString& description)
            : mActive(true)
            , mPercentage(0.)
        {
            setupUi(this);
            setAttribute(Qt::WA_NoSystemBackground);
            resultChanged(Result::Undefined);
            progressBar->setMinimum(0);
            progressBar->setMaximum(100);
            txtHeader->setText( description );
        }

    public:
        void resultChanged(Result result)
        {
            QColor barColor;

            switch (result) {
            case Result::Ok:
                barColor = QColor(0x55, 0xFF, 0x55);
                break;

            case Result::Error:
                barColor = QColor(0xFF, 0x55, 0x55);
                break;

            case Result::Undefined:
            default:
                barColor = QColor(0x99, 0x99, 0x99);
                break;
            }

            progressBar->setStyleSheet(
                        QStringLiteral(
                            "QProgressBar{"
                            "text-align: center;"
                            "border: 2px solid silver;"
                            "border-radius: 5px;"
                            "background: QLinearGradient("
                            "x1: 0, y1: 0, x2: 0, y2: 1,"
                            "stop: 0 %1,"
                            "stop: 1 %2);"
                            "}")
                        .arg(barColor.darker(150).name()).arg(barColor.name()) %

                        QStringLiteral(
                            "QProgressBar::chunk{"
                            "background: QLinearGradient("
                            "x1: 0, y1: 0, x2: 0, y2: 1,"
                            "stop: 0 %1,"
                            "stop: 1 %2);"
                            "}")
                        .arg(barColor.lighter(150).name()).arg(barColor.name())
                        );
        }

    public:
        bool    mActive;
        qreal   mPercentage;

        Steps   mSteps;
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
}

void ProgressDlg::setStatusInfo(QObject* activity, const QString& step,
                                const QString& text)
{
    Private::ProgressWdgt* s = findStep(activity, step);
    Q_ASSERT(s);

    s->progressBar->setFormat(text);
}

void ProgressDlg::setPercentage(QObject* activity, const QString& step,
                                qreal percent)
{
    Private::ProgressWdgt* s = findStep(activity, step);
    Q_ASSERT(s);

    s->mPercentage = qMin( qMax(percent, 0.), 1. ) * 100.;

    if (s->mPercentage >= 100.) {
        s->resultChanged(Private::ProgressWdgt::Result::Ok);
    }
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

    a->mActive = false;
    a->resultChanged(Private::ProgressWdgt::Result::Ok);
}

void ProgressDlg::setError(QObject* activity, const QString& message)
{
    Private::ProgressWdgt* a = mActivities[activity];
    Q_ASSERT(a);
    a->mActive = false;
    a->progressBar->setFormat(tr("Failed. See tool tip for details."));
    a->setToolTip(tr("<b>Activity failed.</b><hr/>%1").arg(message));
    a->resultChanged(Private::ProgressWdgt::Result::Error);
}

void ProgressDlg::updateActivities()
{
    Activities::Iterator it = mActivities.begin();
    while ( it != mActivities.end() ) {
        Private::ProgressWdgt* a = *it;
        Q_ASSERT(a);

        a->mPercentage = 0.;

        foreach (Private::ProgressWdgt* s, a->mSteps) {
            s->progressBar->setValue(qRound(s->mPercentage));
            qreal stepPercent = s->mPercentage / a->mSteps.size();
            a->mPercentage += qMin( qMax(stepPercent, 0.), 100.);
        }

        a->progressBar->setValue(qRound(a->mPercentage));

        // remove invalid activities after updating
        if (!it.key()) {
            it = mActivities.erase(it);
        }
        else {
            ++it;
        }
    }

    setCanClose();
}

void ProgressDlg::accept()
{
    if (isDone()) {
        BlueSky::Dialog::accept();
    }
}

void ProgressDlg::reject()
{
    if (isDone()) {
        BlueSky::Dialog::reject();
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