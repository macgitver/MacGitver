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
        targetDir += QString::fromUtf8("/%1")
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
    mProgress->show();
    mProgress->setCurrent( clone );

    if( repoName.endsWith( QLatin1String( ".git" ) ) )
        repoName = repoName.left( repoName.length() - 4 );

    if( repoName.lastIndexOf( QChar( L'/' ) ) != -1 )
        repoName.remove( 0, repoName.lastIndexOf( QChar( L'/' ) ) + 1 );

    mAction = tr( "Cloning <b>%1</b>" ).arg( repoName );
    mProgress->beginStep( mAction );
    mStates.clear();
    mStates[ Prepare ] = Current;
    mStates[ Download ] = Open;
    mStates[ Index ] = Open;
    mStates[ Checkout ] = Open;
    updateAction();

    connect( clone, SIGNAL(finished()), this, SLOT(rootCloneFinished()) );
    connect( clone, SIGNAL(transportProgress(quint32,quint32,quint32,quint64)),
             this, SLOT(beginDownloading()) );
    connect( clone, SIGNAL(doneDownloading()), this, SLOT(doneDownload()) );
    connect( clone, SIGNAL(doneIndexing()), this, SLOT(doneIndexing()) );
    connect( clone, SIGNAL(doneCheckout()), this, SLOT(doneCheckout()) );
    clone->execute();
}

void CloneDlg::beginDownloading()
{
    disconnect( sender(), SIGNAL(transportProgress(quint32,quint32,quint32,quint64)),
                this, SLOT(beginDownloading()) );

    mStates[ Prepare ] = Done;
    mStates[ Download ] = Current;
    mStates[ Index ] = Current;
    updateAction();
}

void CloneDlg::doneDownload()
{
    mStates[ Download ] = Done;
    updateAction();
}

void CloneDlg::doneIndexing()
{
    mStates[ Index ] = Done;

    if( mStates.contains( Checkout ) )
        mStates[ Checkout ] = Current;

    updateAction();
}

void CloneDlg::doneCheckout()
{
    mStates[ Checkout ] = Done;
    updateAction();
}

void CloneDlg::rootCloneFinished()
{
    Git::BaseOperation* operation = static_cast<Git::BaseOperation*>( sender() );
    Q_ASSERT( operation );

    if ( operation->result() )
    {
        mProgress->setDone();
        return;
    }

    QMessageBox::critical(mProgress, tr("Errors while cloning repository."),
                          tr("Cloning failed:\nGit Message: %1").arg(operation->result().errorText()));
    mProgress->reject();
}

void CloneDlg::updateAction()
{
    QStringList open, current, done;

    QHash< Tasks, QString > t;
    t.insert( Prepare, tr( "Prepare" ) );
    t.insert( Index, tr( "Indexing" ) );
    t.insert( Download, tr( "Downloading" ) );
    t.insert( Checkout, tr( "Check out" ) );

    foreach( Tasks task, mStates.keys() )
    {
        QStringList* sl = NULL;

        if( mStates[ task ] == Open ) sl = &open;
        else if( mStates[ task ] == Done ) sl = &done;
        else if( mStates[ task ] == Current ) sl = &current;

        if( sl )
            sl->append( t[ task ] );
    }

    mProgress->setAction( mAction, open, current, done );
}
