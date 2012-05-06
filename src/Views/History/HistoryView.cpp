
#include <QVBoxLayout>

#include "Views/History/HistoryView.h"
#include "Views/History/HistoryList.h"

HistoryView::HistoryView()
{
	setViewName( trUtf8( "History" ) );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );
	setLayout( l );

	mList = new HistoryList;
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
