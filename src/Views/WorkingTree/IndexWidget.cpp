
#include <QVBoxLayout>

#include "Core/Index.h"

#include "Views/WorkingTree/IndexWidget.h"
#include "Views/WorkingTree/IndexTree.h"

IndexWidget::IndexWidget()
{
	mTree = new IndexTree;
	mTree->setFrameShape( QFrame::NoFrame );

	QVBoxLayout* l = new QVBoxLayout;
	l->setMargin( 0 );
	l->setSpacing( 0 );
	l->addWidget( mTree );

	setLayout( l );

	setViewName( trUtf8( "Working tree" ) );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	mTree->setRepository( repo );
}
