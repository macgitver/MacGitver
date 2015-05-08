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

#include "CloneRepositoryDlg.hpp"

#include "libMacGitverCore/Config/Config.h"

#include "libGitWrap/Operations/CloneOperation.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Widgets/ProgressDlg.hpp"

#include <QFileDialog>
#include <QMessageBox>


CloneOptionsWdgt::CloneOptionsWdgt()
{
    setupUi(this);
}

void CloneOptionsWdgt::on_txtCloneMode_currentIndexChanged(int index)
{
    // Note: clone modes are fixed
    mCloneMode = static_cast<CloneMode>( index );
    grpSubmodules->setEnabled( mCloneMode == cmCheckout );
}

void CloneOptionsWdgt::on_chkInitSubmodules_toggled(bool checked)
{
    chkSubmodulesRecursive->setEnabled( checked );
    if( !checked )
    {
        chkSubmodulesRecursive->setChecked( false );
    }
}


CloneWdgt::CloneWdgt()
{
    setupUi( this );

    connect(btnBrowseTo,    &QAbstractButton::clicked,
            this,           &CloneWdgt::onBrowse);
}

void CloneWdgt::onBrowse()
{
    QString fn = txtPath->text();
    if( fn.isEmpty() )
    {
        fn = Config::self().get( "Repository/lastUsedDir", QDir::homePath() ).toString();
    }

    QFileDialog* fd = new QFileDialog( this, trUtf8( "Select repository location" ) );

    #ifdef Q_OS_MAC
    fd->setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );
    #else
    fd->setFileMode( QFileDialog::Directory );
    #endif

    fd->setDirectory( fn );
    fd->setAcceptMode( QFileDialog::AcceptSave );
    fd->open( this, SLOT(onBrowseHelper(QString)) );
}

void CloneWdgt::onBrowseHelper( const QString& directory )
{
    if( directory.isEmpty() )
    {
        return;
    }

    Config::self().set( "Repository/lastUsedDir", directory );
    txtPath->setText( directory );
}


CloneDlg::CloneDlg()
    : mCloneWdgt(new CloneWdgt)
    , mCloneOptsWdgt(new CloneOptionsWdgt)
{
    setDialogWidgets(mCloneWdgt, mCloneOptsWdgt);

    connect(mCloneWdgt->txtPath,    &QLineEdit::textChanged,
            this,                   &CloneDlg::checkValid);

    checkValid();
}

void CloneDlg::checkValid()
{
    if (!mCloneWdgt) {
        setAcceptable(false);
        return;
    }

    bool okay = !mCloneWdgt->txtPath->text().isEmpty() &&
                !mCloneWdgt->txtUrl->text().isEmpty();

    QDir wanted( QDir::toNativeSeparators( mCloneWdgt->txtPath->text() ) );
    if( wanted.exists() )
    {
        QStringList sl = wanted.entryList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
        if( sl.count() > 0 )
        {
            okay = false;
        }
    }

    setAcceptable( okay );
}

void CloneDlg::accept()
{
    Git::CloneOperation* clone = new Git::CloneOperation( this );
    QString repoName  = QUrl(mCloneWdgt->txtUrl->text() )
                        .adjusted(QUrl::NormalizePathSegments |
                                  QUrl::StripTrailingSlash ).toString();
    QString targetDir = QUrl(mCloneWdgt->txtPath->text() )
                        .adjusted(QUrl::NormalizePathSegments |
                                  QUrl::StripTrailingSlash ).toString();

    if ( mCloneWdgt->chkAppendRepoName->isChecked() )
    {
        targetDir += QStringLiteral("/%1")
                    .arg( QUrl( repoName ).fileName() );
    }

    clone->setBackgroundMode( true );
    clone->setUrl( repoName );
    clone->setPath( targetDir );
    clone->setRemoteAlias( mCloneWdgt->txtRemoteAlias->text() );

    if ( mCloneOptsWdgt )
    {
        clone->setBare( mCloneOptsWdgt->mCloneMode == CloneOptionsWdgt::cmBare );
        clone->setReference( mCloneOptsWdgt->txtBranch->text() );
        clone->setDepth( mCloneOptsWdgt->txtCloneDepth->value() );
    }

    mProgress = new ProgressDlg;

    // TODO: implement a ProgressDlg::minimumDuration
    mProgress->show();

    if( repoName.endsWith( QStringLiteral( ".git" ) ) )
        repoName = repoName.left( repoName.length() - 4 );

    if( repoName.lastIndexOf( QChar( L'/' ) ) != -1 )
        repoName.remove( 0, repoName.lastIndexOf( QChar( L'/' ) ) + 1 );

    ProgressDlg::StepInfo::List steps;
    steps << ProgressDlg::StepInfo{ QStringLiteral("transfer"), tr("Download Git objects.") }
          << ProgressDlg::StepInfo{ QStringLiteral("index"), tr("Add objects to Git index.") };

    if (!clone->bare()) {
          steps << ProgressDlg::StepInfo{ QStringLiteral("checkout"),
                   tr("Checkout the worktree.") };
          connect( clone, &Git::CloneOperation::doneCheckout,
                   this,  &CloneDlg::doneCheckout );
    }

    mProgress->addActivity(tr("Clone <b>%1</b> to <b>%2</b>")
                           .arg(repoName).arg(targetDir), clone, steps);

    connect( clone, &Git::CloneOperation::finished,
             this,  &CloneDlg::rootCloneFinished );
    connect( clone, &Git::CloneOperation::transportProgress,
             this,  &CloneDlg::onTransportProgress );
    connect( clone, &Git::CloneOperation::checkoutProgress,
             this,  &CloneDlg::onCheckoutProgress );
    connect( clone, &Git::CloneOperation::doneDownloading,
             this,  &CloneDlg::doneDownload );
    connect( clone, &Git::CloneOperation::doneIndexing,
             this,  &CloneDlg::doneIndexing );

    clone->execute();
}

void CloneDlg::doneDownload()
{
    mProgress->finished(sender(), QStringLiteral("transfer"));
}

void CloneDlg::doneIndexing()
{
    mProgress->finished(sender(), QStringLiteral("index"));
}

void CloneDlg::doneCheckout()
{
    mProgress->finished(sender(), QStringLiteral("checkout"));
}

void CloneDlg::rootCloneFinished()
{
    Git::BaseOperation* operation = static_cast<Git::BaseOperation*>( sender() );
    Q_ASSERT( operation );

    if ( operation->result() ) {
        mProgress->finished(operation);
    }
    else {
        mProgress->setError(operation, operation->result().errorText());
    }

}

void CloneDlg::onCheckoutProgress(const QString& fileName, quint64 totalFiles,
                                  quint64 completedFiles)
{
    QObject* s = sender();

    const QString stepCheckout( QStringLiteral("checkout") );
    mProgress->setPercentage( s, stepCheckout,
                              (qreal)completedFiles / totalFiles );
    mProgress->setStatusInfo( s, stepCheckout,
                              tr("Checked out %1 of %2 files (last file: %3).")
                              .arg(completedFiles).arg(totalFiles).arg(fileName) );
}

void CloneDlg::onTransportProgress(quint32 totalObjects,
                                   quint32 indexedObjects,
                                   quint32 receivedObjects,
                                   quint64 receivedBytes)
{
    QString recv;
    if( receivedBytes > 1024 * 1024 * 950 ) /* 950 is so we get 0.9 gb */
    {
        recv = QString::number( receivedBytes / (1024*1024*1024.0), 'f', 2 ) + QStringLiteral(" Gb");
    }
    else if( receivedBytes > 1024 * 950 )
    {
        recv = QString::number( receivedBytes / (1024*1024.0), 'f', 2 ) + QStringLiteral(" Mb");
    }
    else if( receivedBytes > 950 )
    {
        recv = QString::number( receivedBytes / 1024.0, 'f', 2 ) + QStringLiteral(" Kb");
    }
    else
    {
        recv = QString::number( receivedBytes );
    }

    QObject* s = sender();
    const QString stepTransfer(QStringLiteral("transfer"));
    mProgress->setPercentage( s, stepTransfer,
                              (qreal)receivedObjects / totalObjects );
    mProgress->setStatusInfo( s, stepTransfer,
                              tr("Received %1 of %2 objects (%3).")
                              .arg(receivedObjects)
                              .arg(totalObjects)
                              .arg(recv) );

    const QString stepIndex(QStringLiteral("index"));
    mProgress->setPercentage( s, stepIndex,
                              (qreal)indexedObjects / totalObjects );
    mProgress->setStatusInfo( s, stepIndex, tr("Indexed %1 of %2 objects.")
                              .arg(indexedObjects)
                              .arg(totalObjects) );
}
