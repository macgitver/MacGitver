
#include <QListWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "Views/Branches/BranchesView.h"

BranchesView::BranchesView()
{
	mListWidget = new QListWidget();
	mListWidget->setFrameStyle( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );

	mToolBar = new QToolBar;

	mBtnLocals = new QToolButton;
	mBtnLocals->setText( trUtf8( "Show local" ) );
	mBtnLocals->setCheckable( true );
	mBtnLocals->setChecked( true );
	connect( mBtnLocals, SIGNAL(toggled(bool)), SLOT(rereadBranches()) );

	mBtnRemotes = new QToolButton;
	mBtnRemotes->setText( trUtf8( "Show remote" ) );
	mBtnRemotes->setCheckable( true );
	mBtnRemotes->setChecked( true );
	connect( mBtnRemotes, SIGNAL(toggled(bool)), SLOT(rereadBranches()) );

	mToolBar->addWidget( mBtnLocals );
	mToolBar->addWidget( mBtnRemotes );

	l->addWidget( mToolBar );
	l->addWidget( mListWidget );

	setLayout( l );

	setViewName( trUtf8( "Branches" ) );
}

void BranchesView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	rereadBranches();
}

void BranchesView::rereadBranches()
{
	mListWidget->clear();

	if( mRepo.isValid() )
	{
		QStringList sl = mRepo.branches( mBtnLocals->isChecked(), mBtnRemotes->isChecked() );

		for( int i = 0; i < sl.count(); i++ )
		{
			new QListWidgetItem( sl[ i ].mid( 5 ), mListWidget );
		}

	}
}
