/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
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

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QStringBuilder>

#include "libMacGitverCore/Config/Config.h"

#include "libGitWrap/Operations/CloneOperation.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"

#include "CloneRepositoryDlg.h"
#include "ProgressDlg.hpp"

CloneRepositoryDlg::CloneRepositoryDlg()
    : mProgress( NULL )
{
    setupUi( this );

    connect( btnBrowse, SIGNAL(clicked()), SLOT(onBrowse()) );
    connect( txtPath, SIGNAL(textChanged(QString)), SLOT(checkValid()) );
    connect( chkCheckout, SIGNAL(toggled(bool)), this, SLOT(onCheckout(bool)) );
    connect( chkInitSubmodules, SIGNAL(toggled(bool)), this, SLOT(onInitSubmodules(bool)) );
    connect( txtCheckoutBranch, SIGNAL(textChanged(QString)), this, SLOT(onCheckoutBranch(QString)) );

    checkValid();
}

void CloneRepositoryDlg::onBrowse()
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

void CloneRepositoryDlg::onBrowseHelper( const QString& directory )
{
    if( directory.isEmpty() )
    {
        return;
    }

    Config::self().set( "Repository/lastUsedDir", directory );
    txtPath->setText( directory );
}

void CloneRepositoryDlg::onCheckout( bool value )
{
    txtCheckoutBranch->setEnabled( value );
    chkInitSubmodules->setEnabled( value );

    if( !value )
    {
        chkInitSubmodules->setChecked( false );
        txtCheckoutBranch->setText( QString() );
    }
}

void CloneRepositoryDlg::onInitSubmodules( bool value )
{
    chkSubmodulesRecursive->setEnabled( value );
    if( !value )
    {
        chkSubmodulesRecursive->setChecked( false );
    }
}

void CloneRepositoryDlg::onCheckoutBranch( const QString& branch )
{
    if( branch.isEmpty() )
    {
        chkFetchOne->setDisabled( true );
        chkFetchOne->setChecked( false );
    }
    else
    {
        chkFetchOne->setEnabled( true );
    }
}

void CloneRepositoryDlg::checkValid()
{
    bool okay =
            !txtPath->text().isEmpty() &&
            !txtUrl->text().isEmpty() &&
            !txtRemoteName->text().isEmpty();

    QDir wanted( QDir::toNativeSeparators( txtPath->text() ) );
    if( wanted.exists() )
    {
        QStringList sl = wanted.entryList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
        if( sl.count() > 0 )
        {
            okay = false;
        }
    }

    buttonBox->button( QDialogButtonBox::Ok )->setEnabled( okay );
}

void CloneRepositoryDlg::accept()
{
    Git::CloneOperation* clone = new Git::CloneOperation( this );
    QString repoName = txtUrl->text();
    QString targetDir = txtPath->text();
    clone->setBackgroundMode( true );
    clone->setUrl( repoName );
    clone->setPath( targetDir );

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
    clone->execute();
}

void CloneRepositoryDlg::beginDownloading()
{
    disconnect( sender(), SIGNAL(transportProgress(quint32,quint32,quint32,quint64)),
                this, SLOT(beginDownloading()) );

    mStates[ Prepare ] = Done;
    mStates[ Download ] = Current;
    mStates[ Index ] = Current;
    updateAction();
}

void CloneRepositoryDlg::doneIndexing()
{
    mStates[ Index ] = Done;

    if( mStates.contains( Checkout ) )
        mStates[ Checkout ] = Current;

    updateAction();
}

void CloneRepositoryDlg::doneCheckout()
{
    mStates[ Checkout ] = Done;
    updateAction();
}

void CloneRepositoryDlg::doneDownload()
{
    mStates[ Download ] = Done;
    updateAction();
}

void CloneRepositoryDlg::updateAction()
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

void CloneRepositoryDlg::rootCloneFinished()
{
    Git::BaseOperation* operation = static_cast<Git::BaseOperation*>( sender() );
    Q_ASSERT( operation );
    if ( !operation->result() )
    {
        QMessageBox::critical(this, tr("Errors while cloning repository."),
                              tr("Cloning failed:\nGit Message: %1").arg(operation->result().errorText()));
        mProgress->reject();
        return;
    }

    if( !chkInitSubmodules->isChecked() )
    {
        if( mStates[ Checkout ] == Current ) doneCheckout();

        mProgress->setDone();

        connect( mProgress, SIGNAL(rejected()), this, SLOT(reject()) );
        return;
    }
}
