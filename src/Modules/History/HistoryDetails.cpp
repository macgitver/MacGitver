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

#include "Config/Config.h"

#include "GitWrap/ObjectCommit.h"
#include "GitWrap/Result.h"

#include "MacGitver/MacGitver.h"

#include "HistoryDetails.h"

HistoryDetails::HistoryDetails( QWidget* parent )
	: QAbstractScrollArea( parent )
{
	setFrameShape( NoFrame );

	readConfig();
}

void HistoryDetails::readConfig()
{
	QVariant vList = Config::self().get( "History/Details/List", QLatin1String( "#" ) );
	QString sList = vList.toString();

	if( sList == QLatin1String( "#" ) )
	{
		mViewDetailRows << HHD_Subject << HHD_AuthorName << HHD_AuthorMail << HHD_AuthorDate
						   << HHD_SHA1 << HHD_ChildrenList << HHD_ParentsList;
	}
	else
	{
		QStringList details = sList.split( QChar( L';' ) );

		foreach( QString s, details )
		{
			mViewDetailRows.append( HistoryHeaderDetails( s.toInt() ) );
		}
	}

	mViewDetails = Config::self().get( "History/Details/Details", true ).toBool();

	setCommit( mCurrentSHA1 );
}

void HistoryDetails::mouseMoveEvent( QMouseEvent* ev )
{
}

void HistoryDetails::mousePressEvent( QMouseEvent* ev )
{
}

void HistoryDetails::mouseReleaseEvent( QMouseEvent* ev )
{
}


void HistoryDetails::paintEvent( QPaintEvent* ev )
{
	QPainter p( viewport() );
	QPalette pal( palette() );

	if( mCurrentSHA1.isNull() )
	{
		return;
	}

	QFont fontDefault = Config::defaultFont();
	QFont fontFixed = Config::defaultFixedFont();

	QFontMetrics fmDefault( fontDefault );
	QFontMetrics fmFixed( fontFixed );

	p.fillRect( mHeader, pal.color( QPalette::Highlight ) );
	p.setPen( Qt::black );
	p.drawRect( mHeader );
	p.setPen( pal.color( QPalette::HighlightedText ) );

	int t = 3;
	for( int i = 0; i < mHeaders.count(); i++ )
	{
		bool useFixedFont = mHeaders[ i ].mFixedFont;
		QFontMetrics& fm = ( useFixedFont ? fmFixed : fmDefault );
		p.setFont( fontDefault );
		p.drawText( 6, mHeader.top() + t, mParamNameWidth, fm.lineSpacing(), 0, mHeaders[ i ].mParameter );

		QRect r1( mHeader.left() + 3, mHeader.top() + t, mHeader.width() - mParamNameWidth - 9, 5000 );
		QRect r2 = fm.boundingRect( r1, Qt::TextWordWrap, mHeaders[ i ].mValue );

		p.setFont( useFixedFont ? fontFixed : fontDefault );
		p.drawText( r2.adjusted( mHeader.left() + mParamNameWidth + 6, 0, mHeader.left() + mParamNameWidth + 6, 0 ), Qt::TextWordWrap, mHeaders[ i ].mValue );

		t += qMax( fm.lineSpacing(), r2.height() + 3 );
	}

	p.setPen( pal.color( QPalette::Text ) );
	p.setFont( fontFixed );

	int top = mDetailsRect.top();
	for( int i = 0; i < mDetails.count(); i++ )
	{
		QRect r = QRect( 10, top, mDetailsRect.width(), fmFixed.lineSpacing() );
		p.drawText( r, mDetails[ i ] );
		top += fmFixed.lineSpacing();
	}
}

void HistoryDetails::resizeEvent( QResizeEvent* ev )
{
	calculate();
	QAbstractScrollArea::resizeEvent( ev );
}

void HistoryDetails::calculate()
{
	QFont font = Config::defaultFont();
	QFont fontFixed = Config::defaultFixedFont();

	QFontMetrics fm( font );
	QFontMetrics fmFixed( fontFixed );

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

	int minWidth = 0;
	for( int i = 0; i < mDetails.count(); i++ )
	{
		int width = fmFixed.width( mDetails[ i ] );
		minWidth = qMax( width, minWidth );
	}

	mDetailsRect = QRect( 3, 10 + headheight, minWidth, mDetails.count() * fmFixed.lineSpacing() );

	viewport()->update();
}

void HistoryDetails::clear()
{
	mCurrentSHA1 = Git::ObjectId();
	mHeaders.clear();
	mDetails.clear();
	calculate();
}

void HistoryDetails::setRepository( Git::Repository repo )
{
	mRepo = repo;
	clear();
}

void HistoryDetails::setCommit( const Git::ObjectId& sha1 )
{
	Git::Result r;

	Q_ASSERT( mRepo.isValid() || sha1.isNull() );

	clear();
	if( sha1.isNull() )
	{
		return;
	}

	mCurrentSHA1 = sha1;

	Git::ObjectCommit commit = mRepo.lookupCommit( sha1, r );
	if( !r )
	{
		MacGitver::self().log( ltError, r, "Reading commit details" );
		clear();
		return;
	}

	for( int i = 0; i < mViewDetailRows.count(); i++ )
	{
		switch( mViewDetailRows[ i ] )
		{
		case HHD_Subject:
			mHeaders.append( HeaderEntry( trUtf8( "Subject" ),
										  commit.shortMessage( r ) ) );
			break;

		case HHD_Author:
			mHeaders.append( HeaderEntry( trUtf8( "Author" ),
										  commit.author( r ).fullName() ) );
			break;

		case HHD_AuthorDate:
			mHeaders.append( HeaderEntry( trUtf8( "Author date" ),
										  commit.author( r ).when().toString() ) );
			break;

		case HHD_AuthorMail:
			mHeaders.append( HeaderEntry( trUtf8( "Author mail" ),
										  commit.author( r ).email() ) );
			break;

		case HHD_AuthorName:
			mHeaders.append( HeaderEntry( trUtf8( "Author name" ),
										  commit.author( r ).name() ) );
			break;

		case HHD_Committer:
			mHeaders.append( HeaderEntry( trUtf8( "Committer" ),
										  commit.committer( r ).fullName() ) );
			break;

		case HHD_CommitterDate:
			mHeaders.append( HeaderEntry( trUtf8( "Committer date" ),
										  commit.committer( r ).when().toString() ) );
			break;

		case HHD_CommitterMail:
			mHeaders.append( HeaderEntry( trUtf8( "Committer mail" ),
										  commit.committer( r ).email() ) );
			break;

		case HHD_CommitterName:
			mHeaders.append( HeaderEntry( trUtf8( "Committer name" ),
										  commit.committer( r ).name() ) );
			break;

		case HHD_SHA1:
			mHeaders.append( HeaderEntry( trUtf8( "SHA-1" ), sha1.toString(), true ) );
			break;

		default:
			break;
		}

		if( !r )
		{
			MacGitver::self().log( ltError, r, "Reading commit details" );
			r.clear();
		}
	}

	mDetails = commit.message( r ).split( QChar( L'\n' ) );
	mDetails.removeFirst();

	calculate();
}
