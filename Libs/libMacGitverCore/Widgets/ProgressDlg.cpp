
#include "ProgressDlg.hpp"
#include "ui_ProgressDlg.h"
#include "ui_ProgressWdgt.h"

#include <QStringBuilder>
#include <QCloseEvent>
#include <QDebug>
#include <QPushButton>
#include <QString>

namespace Private
{

    class ProgressWdgt : public QWidget, public Ui::ProgressWdgt
    {
    public:
        typedef QMap< QString, QPointer<ProgressWdgt> > Steps;

        enum Status { Running = 0, Stopped };

    public:
        ProgressWdgt(const QString& description)
        {
            setupUi(this);
            progressBar->setMinimum(0);
            progressBar->setMaximum(100);
            txtHeader->setText( description );
        }

    public:
        Steps   mSteps;
        Status  mStatus     = Running;

        qreal   mPercentage = 0.;
    };

}


ProgressDlg::ProgressDlg()
    : BlueSky::Dialog()
    , ui( new Ui::ProgressDlg )
    , mDone( false )
{
    ui->setupUi( this );

    QPushButton* close = ui->buttonBox->button( QDialogButtonBox::Close );
    close->setEnabled( false );
    connect( close, &QPushButton::clicked, this, &ProgressDlg::close );

    QPalette p;
    p.setColor( QPalette::Base, p.color( QPalette::Window ) );
    p.setColor( QPalette::Text, p.color( QPalette::WindowText ) );
    ui->txtLog->setPalette( p );
}

ProgressDlg::~ProgressDlg()
{
    delete ui;
}

void ProgressDlg::setAction( const QString& action,
                             const QStringList& open,
                             const QStringList& current,
                             const QStringList& done )
{
    QString act = action;

    foreach( QString s, done )
    {
        act += QStringLiteral( " (<font color=\"green\">" ) % s % QStringLiteral( "</font>)" );
    }

    foreach( QString s, current )
    {
        act += QStringLiteral( " (<font color=\"blue\">" ) % s % QStringLiteral( "</font>)" );
    }

    foreach( QString s, open )
    {
        act += QStringLiteral( " (<font color=\"red\">" ) % s % QStringLiteral( "</font>)" );
    }

    ui->lblAction->setText( act );
}

void ProgressDlg::setCurrent(QObject* current)
{
    mCurrent = current;

    connect( mCurrent, SIGNAL(remoteMessage(QString)),
             this, SLOT(remoteMessage(QString)) );
    connect( mCurrent, SIGNAL(transportProgress(quint32, quint32, quint32, quint64)),
             this, SLOT(transportProgress(quint32, quint32, quint32, quint64)) );
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

void ProgressDlg::transportProgress( quint32 totalObjects,
                                     quint32 indexedObjects,
                                     quint32 receivedObjects,
                                     quint64 receivedBytes )
{
    QString recv;
    if( receivedBytes > 1024 * 1024 * 950 ) /* 950 is so we get 0.9 gb */
    {
        recv = QString::number( receivedBytes / (1024*1024*1024.0), 'f', 2 ) % QStringLiteral( " Gb" );
    }
    else if( receivedBytes > 1024 * 950 )
    {
        recv = QString::number( receivedBytes / (1024*1024.0), 'f', 2 ) % QStringLiteral( " Mb" );
    }
    else if( receivedBytes > 950 )
    {
        recv = QString::number( receivedBytes / 1024.0, 'f', 2 ) % QStringLiteral( " Kb" );
    }
    else
    {
        recv = QString::number( receivedBytes );
    }
    ui->lblTransferSize->setText( recv );

    ui->progressBar->setRange( 0, totalObjects * 2 );
    ui->progressBar->setValue( indexedObjects + receivedObjects );
}

void ProgressDlg::remoteMessage( const QString& msg )
{
    mRawRemoteMessage += msg;

    QString output;
    QChar outputBuffer[ 256 ];
    int outBufPos = 0, outBufLen = 0;

    for( int i = 0; i < mRawRemoteMessage.length(); ++i )
    {
        if( mRawRemoteMessage[ i ] == QChar( L'\r' ) )
        {
            outBufPos = 0;
        }
        else if( mRawRemoteMessage[ i ] == QChar( L'\n' ) )
        {
            if( outBufLen )
                output += QString( outputBuffer, outBufLen );
            output += QChar( L'\n' );
            outBufPos = outBufLen = 0;
        }
        else
        {
            outputBuffer[ outBufPos++] = mRawRemoteMessage[ i ];
            outBufLen = qMax( outBufLen, outBufPos );
        }
    }

    if( outBufLen )
        output += QString( outputBuffer, outBufLen );

    QString log = mBaseLog % QStringLiteral( "<br/>" ) %
            output.replace( QChar( L'\n' ), QLatin1String("<br/>") ).simplified();

    ui->txtLog->setHtml( log );
}

void ProgressDlg::beginStep( const QString& step )
{
    mBaseLog += tr( "<font color=\"blue\">%1</font><br/>" ).arg( step );
    ui->txtLog->setHtml( mBaseLog );
}

void ProgressDlg::finalizeStep()
{
    mBaseLog = ui->txtLog->toHtml() % QStringLiteral( "<br/>" );
    mRawRemoteMessage = QString();

    ui->txtLog->setHtml( mBaseLog );
}

void ProgressDlg::setDone()
{
    mDone = true;
    ui->buttonBox->button( QDialogButtonBox::Close )->setEnabled( true );
}
