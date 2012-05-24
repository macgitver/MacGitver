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

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>

#include "GitWrap/Reference.h"
#include "GitWrap/ObjectCommit.h"

#include "HistoryView.h"
#include "HistoryEntry.h"
#include "HistoryModel.h"
#include "HistoryBuilder.h"
#include "HistoryList.h"

#define SLOT_X 12
#define SLOT_Y 12

void HistoryViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option,
								 const QModelIndex& index ) const
{
	static QColor colors[] = { Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan };
	static const int numColors = sizeof( colors ) / sizeof( colors[0] );

	if( index.column() != 0 )
	{
		QItemDelegate::paint( painter, option, index );
		return;
	}

	drawBackground( painter, option, index );

	QRect rect = option.rect;
	painter->save();
	painter->setRenderHint( QPainter::Antialiasing );
	painter->setClipRect( rect );

	HistoryModel* m = const_cast< HistoryModel* >( qobject_cast< const HistoryModel* >( index.model() ) );
	HistoryEntry* e = m->entries()->at( index.row() );

	int slot = e->slot();
	QRect subSlot( rect.left() + slot * SLOT_X, rect.top() + ( rect.height() - SLOT_Y ) / 2,
				   SLOT_X, SLOT_Y );


	HistoryGraphDrawInfos& infos = e->drawInfos();
	for( int j = 0; j < infos.count(); j++ )
	{
		HistoryGraphDrawInfo i = infos.at( j );

		painter->setPen( QPen( colors[ i.endColumn % numColors ], 1 ) );

		if( i.endColumn == i.startColumn )
		{
			QLine l( rect.left() + i.startColumn * SLOT_X + SLOT_X / 2,
					 rect.top() + ( i.endsUpwards ? 0 : rect.height() / 2 ),
					 rect.left() + i.startColumn * SLOT_X + SLOT_X / 2,
					 rect.top() + ( i.endsDownwards ? rect.height() : rect.height() / 2 ) );
			painter->drawLine( l );
		}
		else if( i.endColumn > i.startColumn && i.endsUpwards )
		{
			QLine l1( rect.left() + i.startColumn * SLOT_X + SLOT_X / 2,
					  rect.top() + rect.height() / 2,
					  rect.left() + i.endColumn * SLOT_X,
					  rect.top() + rect.height() / 2 );

			QLine l2( rect.left() + i.endColumn * SLOT_X,
					  rect.top() + rect.height() / 2,
					  rect.left() + i.endColumn * SLOT_X + SLOT_X / 2,
					  rect.top() );

			painter->drawLine( l1 );
			painter->drawLine( l2 );
		}
		else if( i.endColumn > i.startColumn && i.endsDownwards )
		{
			QLine l1( rect.left() + i.startColumn * SLOT_X + SLOT_X / 2,
					  rect.top() + rect.height() / 2,
					  rect.left() + i.endColumn * SLOT_X,
					  rect.top() + rect.height() / 2 );

			QLine l2( rect.left() + i.endColumn * SLOT_X,
					  rect.top() + rect.height() / 2,
					  rect.left() + i.endColumn * SLOT_X + SLOT_X / 2,
					  rect.top() + rect.height() );

			painter->drawLine( l1 );
			painter->drawLine( l2 );
		}
	}

	painter->setPen( QPen( Qt::black, 1 ) );
	painter->fillRect( subSlot.adjusted( 2, 2, -2, -2 ), colors[ slot % numColors ] );
	painter->drawRect( subSlot.adjusted( 2, 2, -2, -2 ) );

	drawFocus( painter, option, option.rect );

	painter->restore();
}


HistoryView::HistoryView()
{
	setViewName( trUtf8( "History" ) );

	QVBoxLayout* l = new QVBoxLayout;
	l->setSpacing( 2 );
	l->setMargin( 2 );
	setLayout( l );

	mToolBar = new QToolBar;

	QAction* a = mToolBar->addAction( trUtf8( "All Branches" ), this, SLOT(onViewAllBranches()) );
	a->setCheckable( true );

	l->addWidget( mToolBar );

	mList = new HistoryList;
	mList->setFrameShape( QFrame::NoFrame );
	l->addWidget( mList );

	mModel = new HistoryModel( this );
	mList->setModel( mModel );

	mList->setItemDelegate( new HistoryViewDelegate );

	mBuilder = NULL;
}

void HistoryView::repositoryChanged( Git::Repository repo )
{
	if( mBuilder )
	{
		delete mBuilder;
		mBuilder = NULL;
	}

	mRepo = repo;

	if( mRepo.isValid() )
	{
		buildHistory();
	}
}

void HistoryView::buildHistory()
{
	if( !mRepo.isValid() )
	{
		return;
	}

	if( !mBuilder )
	{
		mBuilder = new HistoryBuilder( mRepo, mModel->entries() );
	}

	mBuilder->addHEAD();
	mBuilder->start();
}
