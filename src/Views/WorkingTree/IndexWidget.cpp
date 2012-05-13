
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "Core/Index.h"

#include "Views/WorkingTree/IndexWidget.h"
#include "Views/WorkingTree/IndexTree.h"

IndexWidget::IndexWidget()
{
	mTree = new IndexTree;
	mTree->setFrameShape( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );

	mToolBar = new QToolBar;

	mactShowAll = mToolBar->addAction( trUtf8( "All" ), this, SLOT(onShowAll(bool)) );
	mactShowAll->setCheckable( true );

	mactShowUnchanged = mToolBar->addAction( trUtf8( "Unchanged" ), this, SLOT(onShowUnchanged(bool)) );
	mactShowUnchanged->setCheckable( true );

	mactShowModified = mToolBar->addAction( trUtf8( "Modified" ), this, SLOT(onShowModified(bool)) );
	mactShowModified->setCheckable( true );

	mactShowMissing = mToolBar->addAction( trUtf8( "Missing" ), this, SLOT(onShowMissing(bool)) );
	mactShowMissing->setCheckable( true );

	mactShowIgnored = mToolBar->addAction( trUtf8( "Ignored" ), this, SLOT(onShowIgnored(bool)) );
	mactShowIgnored->setCheckable( true );

	mactShowUntacked = mToolBar->addAction( trUtf8( "Untracked" ), this, SLOT(onShowUntracked(bool)) );
	mactShowUntacked->setCheckable( true );

	mToolBar->addSeparator();

	mToolBar->addAction( "Stage" );
	mToolBar->addAction( "Commit" );
	mToolBar->addAction( "Reset" );

	l->addWidget( mToolBar );

	l->addWidget( mTree );
	setLayout( l );

	setViewName( trUtf8( "Working tree" ) );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	mTree->setRepository( repo );
}

void IndexWidget::onShowAll( bool enabled )
{
}

void IndexWidget::onShowModified( bool enabled )
{
}

void IndexWidget::onShowMissing( bool enabled )
{
}

void IndexWidget::onShowIgnored( bool enabled )
{
}

void IndexWidget::onShowUntracked( bool enabled )
{
}

void IndexWidget::onShowUnchanged( bool enabled )
{
}

