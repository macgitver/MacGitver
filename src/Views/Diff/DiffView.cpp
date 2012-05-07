
#include <QHBoxLayout>

#include "Views/Diff/Diff.h"
#include "Views/Diff/DiffDisplay.h"
#include "Views/Diff/DiffTreeFileList.h"
#include "Views/Diff/DiffView.h"

DiffView::DiffView()
	: mPatch( NULL )
	, mDisplay( NULL )
	, mTree( NULL )
{
	setViewName( trUtf8( "Differences" ) );

	mPatch = Differences::readPatch( "/work/test.diff" );

	mDisplay = new DiffDisplayWidget();

	QHBoxLayout* l = new QHBoxLayout;
	l->setMargin( 0 );
	l->setSpacing( 0 );

	l->addWidget( mDisplay );
	setLayout( l );


	mDisplay->setDifference( mPatch->pathAt( 0 ) );
}

DiffView::~DiffView()
{
	delete mPatch;
}

