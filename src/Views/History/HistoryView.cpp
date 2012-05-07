
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>

#include "Views/History/HistoryView.h"
#include "Views/History/HistoryList.h"

HistoryView::HistoryView()
{
	setViewName( trUtf8( "History" ) );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );
	setLayout( l );

	mToolBar = new QToolBar;

	QAction* a = mToolBar->addAction( trUtf8( "All Branches" ), this, SLOT(onViewAllBranches()) );
	a->setCheckable( true );

	l->addWidget( mToolBar );

	mList = new HistoryList;
	mList->setFrameShape( QFrame::NoFrame );
	l->addWidget( mList );

	mModel = new HistoryModel( this );
	mList->setModel( mModel );
}

void HistoryView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
}

void HistoryView::buildHistory()
{
}
