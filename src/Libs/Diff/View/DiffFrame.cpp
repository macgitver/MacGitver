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

#include <QGridLayout>
#include <QScrollBar>

#include "Diff/Model/Diff.h"
#include "Diff/Model/Patch.h"

#include "DiffFrame.h"
#include "DiffWidget.h"
#include "DiffSplitter.h"

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

