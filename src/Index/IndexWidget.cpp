
#include <QVBoxLayout>

#include "Core/Index.h"

#include "Index/IndexWidget.h"
#include "Index/IndexTree.h"

IndexWidget::IndexWidget()
{
	mTree = new IndexTree;

	QVBoxLayout* l = new QVBoxLayout;
	l->setMargin( 0 );
	l->setSpacing( 0 );
	l->addWidget( mTree );

	setLayout( l );
}

void IndexWidget::repositoryChanged( Git::Repository repo )
{
	mRepo = repo;

	Git::Index i = mRepo.index();
	mTree->setIndex( i );
}
