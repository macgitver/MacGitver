
#include <QListWidget>
#include <QVBoxLayout>

#include "Views/Refs/ReferenceView.h"

ReferenceView::ReferenceView()
{
	mListWidget = new QListWidget();
	mListWidget->setFrameStyle( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );
	l->addWidget( mListWidget );

	setLayout( l );

	setViewName( trUtf8( "Refs" ) );
}

void ReferenceView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	mListWidget->clear();
	QStringList sl = mRepo.allReferences();

	for( int i = 0; i < sl.count(); i++ )
	{
		new QListWidgetItem( sl[ i ].mid( 5 ), mListWidget );
	}
}
