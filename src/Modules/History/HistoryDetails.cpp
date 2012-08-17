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

#include "GitWrap/ObjectCommit.h"

#include "HistoryDetails.h"

HistoryDetails::HistoryDetails( QWidget* parent )
	: QAbstractScrollArea( parent )
{
	setFrameShape( NoFrame );
	calculate();
}


void HistoryDetails::paintEvent( QPaintEvent* ev )
{
	QPainter p( viewport() );

	qDebug() << mHeader;

	QFontMetrics fm( font() );

	p.fillRect( mHeader, Qt::gray );
	p.setPen( Qt::black );
	p.drawRect( mHeader );
	p.setPen( Qt::white );

	int t = 3;
	for( int i = 0; i < mHeaders.count(); i++ )
	{
		p.drawText( 6, mHeader.top() + t, mParamNameWidth, fm.lineSpacing(), 0, mHeaders[ i ].mParameter );
		QRect r1( mHeader.left() + 3, mHeader.top() + t, mHeader.width() - mParamNameWidth - 9, 5000 );
		QRect r2 = fm.boundingRect( r1, Qt::TextWordWrap, mHeaders[ i ].mValue );

		p.drawText( r2.adjusted( mHeader.left() + mParamNameWidth + 6, 0, mHeader.left() + mParamNameWidth + 6, 0 ), Qt::TextWordWrap, mHeaders[ i ].mValue );

		t += qMax( fm.lineSpacing(), r2.height() + 3 );
	}
}

void HistoryDetails::resizeEvent( QResizeEvent* ev )
{
	calculate();
	QAbstractScrollArea::resizeEvent( ev );
}

void HistoryDetails::calculate()
{
	QFontMetrics fm( font() );

	int w = width();

	mParamNameWidth = 0;
	for( int i = 0; i < mHeaders.count(); i++ )
	{
		int w2 = fm.width( mHeaders[ i ].mParameter );
		mParamNameWidth = qMax( w2, mParamNameWidth );
	}

	int valueWidth = w - 3 - 3 - 3 - 3 - 3 - mParamNameWidth;
	int headheight = 0;
	for( int i = 0; i < mHeaders.count(); i++ )
	{
		QRect r( 0, 0, valueWidth, 50000 );
		QRect r2 = fm.boundingRect( r, Qt::TextWordWrap, mHeaders[ i ].mValue );
		headheight += r2.height() + 3;
	}

	mHeader = QRect( 3, 3, w - 3 - 3, headheight );

	viewport()->update();
}

void HistoryDetails::setRepository( Git::Repository repo )
{
	mRepo = repo;
	calculate();
}

void HistoryDetails::setCommit( const Git::ObjectId& sha1 )
{
	Q_ASSERT( mRepo.isValid() );
	mHeaders.clear();

	if( sha1.isNull() )
	{
		calculate();
		return;
	}

	Git::ObjectCommit commit = mRepo.lookupCommit( sha1 );
	Q_ASSERT( commit.isValid() );

	mHeaders.append( HeaderEntry( trUtf8( "Author" ), commit.author().fullName() ) );
	mHeaders.append( HeaderEntry( trUtf8( "Author date" ), commit.author().when().toString() ) );

	mHeaders.append( HeaderEntry( trUtf8( "Committer" ), commit.committer().fullName() ) );
	mHeaders.append( HeaderEntry( trUtf8( "Committer date" ), commit.committer().when().toString() ) );

	mHeaders.append( HeaderEntry( trUtf8( "SHA-1" ), sha1.toString(), true ) );

	calculate();
}
