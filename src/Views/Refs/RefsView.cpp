
#include <QListWidget>
#include <QVBoxLayout>

#include "Views/Refs/RefsView.h"

RefsView::RefsView()
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

void RefsView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;
	mListWidget->clear();

	if( mRepo.isValid() )
	{
		QStringList sl = mRepo.allReferences();

		for( int i = 0; i < sl.count(); i++ )
		{
			new QListWidgetItem( sl[ i ].mid( 5 ), mListWidget );
		}
	}
}
