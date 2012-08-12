/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTextBrowser>

#include "Diff/Model/Diff.h"
#include "Diff/Model/Patch.h"

#include "DiffWidget.h"
#include "DiffTreeFileList.h"
#include "DiffView.h"
#include "DiffSplitter.h"
#include "DiffFrame.h"
#include "Diff/RawView/DiffRawView.h"

DiffView::DiffView()
	: View( trUtf8( "Diff" ) )
	, mPatch( NULL )
	, mTree( NULL )
{
	setFont( QFont( QLatin1String( "Monospace" ), 8 ) );
//	setViewName( trUtf8( "Differences" ) );

	QHBoxLayout* l = new QHBoxLayout;
	l->setMargin( 0 );
	l->setSpacing( 0 );
	setLayout( l );

	QSplitter* s1 = new QSplitter( Qt::Horizontal );
	QSplitter* s2 = new QSplitter( Qt::Vertical );
	mDiffFrame = new DiffFrame();

	s1->addWidget( s2 );

	mDiffStack = new QStackedWidget;
	mRawDiffView = new DiffRawView;
	mDiffStack->addWidget( mDiffFrame );
	mDiffStack->addWidget( mRawDiffView );
	mDiffStack->setCurrentIndex( 1 );

	s1->addWidget( mDiffStack );

	mTree = new DiffTreeFileList;
	mDetails = new QTreeWidget;

	s2->addWidget( mTree );
	s2->addWidget( mDetails );

	l->addWidget( s1 );

//	setPatch( Patch::readPatch( "/work/test.diff" ) );
}

DiffView::~DiffView()
{
}

void DiffView::setPatch( Patch::Ptr patch )
{
	clearTree();

	mDiffFrame->setPatch( patch );

	mRawDiffView->setText( patch ? patch->toString() : QString() );

	mPatch = patch;
}

void DiffView::clearTree()
{

}

void DiffView::fillTree()
{

}
