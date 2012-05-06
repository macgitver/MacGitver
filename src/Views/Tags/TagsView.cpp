
#include <QListWidget>
#include <QVBoxLayout>

#include "Views/Tags/TagsView.h"

TagsView::TagsView()
{
	mListWidget = new QListWidget();
	mListWidget->setFrameStyle( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 0 );
	l->setMargin( 0 );
	l->addWidget( mListWidget );

	setLayout( l );

	setViewName( trUtf8( "Tags" ) );
}

void TagsView::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	mListWidget->clear();

	if( mRepo.isValid() )
	{
		QStringList sl = mRepo.allTags();

		for( int i = 0; i < sl.count(); i++ )
		{
			new QListWidgetItem( sl[ i ], mListWidget );
		}
	}
}
