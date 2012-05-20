/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Views/Diff/DiffWidget.h"
#include "Views/Diff/DiffTreeFileList.h"
#include "Views/Diff/DiffView.h"
#include "Views/Diff/DiffSplitter.h"
#include "Views/Diff/DiffFrame.h"
#include "Views/Diff/DiffRawHighlighter.h"

DiffView::DiffView()
	: mPatch( NULL )
	, mTree( NULL )
{
	setFont( QFont( "Monospace", 8 ) );
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
	mRawDiffView = new QTextBrowser;
	mDiffStack->addWidget( mDiffFrame );
	mDiffStack->addWidget( mRawDiffView );
	mDiffStack->setCurrentIndex( 1 );

	s1->addWidget( mDiffStack );

	mTree = new DiffTreeFileList;
	mDetails = new QTreeWidget;

	s2->addWidget( mTree );
	s2->addWidget( mDetails );

	l->addWidget( s1 );

	new DiffRawHighlighter( mRawDiffView );

	setPatch( Patch::readPatch( "/work/test.diff" ) );
}

DiffView::~DiffView()
{
	delete mPatch;
}

void DiffView::setPatch( Patch* patch )
{
	clearTree();

	mDiffFrame->setPatch( patch );

	mRawDiffView->setText( patch ? patch->toString() : QString() );

	delete mPatch;
	mPatch = patch;
}

void DiffView::clearTree()
{

}

void DiffView::fillTree()
{

}
