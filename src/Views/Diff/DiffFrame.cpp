
#include <QGridLayout>
#include <QScrollBar>

#include "Libs/Diff/Diff.h"
#include "Libs/Diff/Patch.h"

#include "Views/Diff/DiffFrame.h"
#include "Views/Diff/DiffWidget.h"
#include "Views/Diff/DiffSplitter.h"

DiffFrame::DiffFrame()
	: QFrame()
	, mPatch( NULL )
{
	QGridLayout* l = new QGridLayout;

	l->addWidget( mSplitter = new DiffSplitter, 0, 0 );
	l->addWidget( mScrollH = new QScrollBar(Qt::Horizontal), 1, 0 );
	l->addWidget( mScrollV = new QScrollBar(Qt::Vertical), 0, 1 );

	l->setSpacing( 0 );
	l->setMargin( 0 );
	setLayout( l );

	setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
}

void DiffFrame::setPatch( Patch* patch )
{
	foreach( DiffWidget* disp, mWidgets )
	{
		delete disp;
	}

	mPatch = patch;

	if( !mPatch )
	{
		return;
	}

	for( int i = 0; i < mPatch->numSides(); i++ )
	{
		DiffWidget* disp = new DiffWidget;
		mWidgets.append( disp );
		mSplitter->addWidget( disp );
	}
}

