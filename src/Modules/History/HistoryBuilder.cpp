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

#include <QElapsedTimer>

#include "MacGitver/MacGitver.h"

#include "GitWrap/ObjectId.h"
#include "GitWrap/Reference.h"

#include "HistoryBuilder.h"
#include "HistoryEntry.h"

HistoryBuilder::HistoryBuilder( Git::Repository repo, HistoryEntries* entries )
	: mRepo( repo )
	, mEntries( entries )
{
	mWalker = mRepo.newWalker();
	mWalker.setSorting( true, false );
}

HistoryBuilder::~HistoryBuilder()
{
	mEntries->clear();
}

void HistoryBuilder::addHEAD()
{
	mWalker.pushHead();
}

void HistoryBuilder::addAllRefs()
{
	QStringList sl = mRepo.allBranches();
	foreach( QString s, sl )
	{
		mWalker.pushRef( s );
	}
}

int HistoryBuilder::nextParent( const Git::ObjectId& sha1, int start )
{
	for( int i = start; i < mCurrentGlyphs.count(); i++ )
	{
		if( mNextParent.at( i ) == sha1 )
		{
			return i;
		}
	}

	return -1;
}

int HistoryBuilder::createGlyphSlot( GraphGlyphs glyph, const Git::ObjectId& nextSha1InSlot,
									 int start )
{
	if( start < mCurrentGlyphs.count() )
	{
		while( start < mCurrentGlyphs.count() )
		{
			if( mCurrentGlyphs[ start ] == ggCrossUnused || mCurrentGlyphs[ start ] == ggUnused )
			{
				mCurrentGlyphs[ start ] = glyph;
				mNextParent[ start ] = nextSha1InSlot;
				return start;
			}
			start++;
		}
	}

	mCurrentGlyphs.append( glyph );
	mNextParent.append( nextSha1InSlot );

	return mCurrentGlyphs.count() - 1;
}

void HistoryBuilder::start()
{
	QElapsedTimer timer;
	timer.start();

	mEntries->beforeClear();

	Git::ObjectId		currentSHA1;
	Git::ObjectCommit	curCommit;

	mNextParent.clear();
	mCurrentGlyphs.clear();
	mCurrentLine = -1;

	while( mWalker.next( currentSHA1 ) )
	{
		if( mCurrentLine == -1 )
		{
			createGlyphSlot( ggBranch, currentSHA1 );
			mCurrentLine++;
		}
		curCommit = mRepo.lookupCommit( currentSHA1 );

		HistoryEntry* entry = new HistoryEntry( curCommit );
		mEntries->append( entry );

		int numParents = curCommit.numParentCommits();
		bool didFork = false;

		int nextLine = nextParent( currentSHA1 );
		if( nextLine != mCurrentLine )
		{
			GraphGlyphs& curGlyph = mCurrentGlyphs[ mCurrentLine ];

			if( curGlyph == ggInitial )
				curGlyph = ggUnused;
			else
				curGlyph = ggNotCurrent;

			if( nextLine != -1 )
				mCurrentGlyphs[ nextLine ] = ggCurrent;
			else
				nextLine = createGlyphSlot( ggBranch, currentSHA1, mCurrentLine );

			mCurrentLine = nextLine;
		}

		if( nextLine != -1 && nextParent( currentSHA1, nextLine + 1 ) != -1 )
		{
			didFork = true;
			int start, end, cur;
			start = end = cur = nextParent( currentSHA1 );

			while( cur != -1 )
			{
				end = cur;
				mCurrentGlyphs[ cur ] = ggTail;
				cur = nextParent( currentSHA1, cur + 1 );
			}

			mCurrentGlyphs[ mCurrentLine ] = ggMergeFork;

			GraphGlyphs& startGlyph = mCurrentGlyphs[ start ];
			GraphGlyphs& endGlyph = mCurrentGlyphs[ end ];

			if( startGlyph == ggMergeFork )
				startGlyph = ggMergeForkLeft;

			if( endGlyph == ggMergeFork )
				endGlyph = ggMergeForkRight;

			if( startGlyph == ggTail )
				startGlyph = ggTailLeft;

			if( endGlyph == ggTail )
				endGlyph = ggTailRight;

			for( int i = start + 1; i < end; i++ )
			{
				GraphGlyphs& gl = mCurrentGlyphs[ i ];

				if( gl == ggNotCurrent )
					gl = ggCross;
				else if( gl == ggUnused )
					gl = ggCrossUnused;
			}
		}

		if( numParents > 1 )
		{
			GraphGlyphs& gl = mCurrentGlyphs[ mCurrentLine ];

			bool wasFork	= gl == ggMergeFork;
			bool wasForkR	= gl == ggMergeForkRight;
			bool wasForkL	= gl == ggMergeForkLeft;
			bool startedCross = false, endedCross = false;

			gl = ggMergeFork;
			int start = mCurrentLine, end = mCurrentLine;

			for( int j = 1; j < numParents; j++ )
			{
				Git::ObjectId parentSha1 = curCommit.parentCommitId( j );
				int idx = nextParent( parentSha1 );
				if( idx != -1 )
				{
					if( idx > end )
					{
						end = idx;
						endedCross = mCurrentGlyphs[ idx ] == ggCross;
					}
					if( idx < start )
					{
						start = idx;
						startedCross = mCurrentGlyphs[ idx ] == ggCross;
					}

					mCurrentGlyphs[ idx ] = ggJoin;
				}
				else
				{
					end = createGlyphSlot( ggHead, parentSha1, end + 1 );
				}
			}

			GraphGlyphs& startGlyph = mCurrentGlyphs[ start ];
			GraphGlyphs& endGlyph = mCurrentGlyphs[ end ];

			if( startGlyph == ggMergeFork && !wasFork && !wasForkR )
				startGlyph = ggMergeForkLeft;

			if( endGlyph == ggMergeFork && !wasFork && !wasForkL )
				endGlyph = ggMergeForkRight;

			if( startGlyph == ggJoin && !startedCross )
				startGlyph = ggJoinLeft;

			if( endGlyph == ggJoin && !endedCross )
				endGlyph = ggJoinRight;

			if( startGlyph == ggHead )
				startGlyph = ggHeadLeft;

			if( endGlyph == ggHead )
				endGlyph = ggHeadRight;

			for( int i = start + 1; i < end; i++ )
			{
				GraphGlyphs& gl = mCurrentGlyphs[ i ];

				if( gl == ggNotCurrent )
					gl = ggCross;
				else if( gl == ggUnused )
					gl = ggCrossUnused;
				else if( gl == ggTailLeft || gl == ggTailRight )
					gl = ggTail;
			}
		}

		if( numParents == 0 )
		{
			GraphGlyphs& gl = mCurrentGlyphs[ mCurrentLine ];
			if( gl != ggMergeFork && gl != ggMergeForkLeft && gl != ggMergeForkRight )
			{
				gl = ggInitial;
			}
		}

		entry->setGlyphs( mCurrentGlyphs );

		if( numParents == 0 )
		{
			mNextParent[ mCurrentLine ] = Git::ObjectId();
		}
		else
		{
			mNextParent[ mCurrentLine ] = curCommit.parentCommitId( 0 );
		}

		if( numParents > 1 )
		{
			for( int i = 0; i < mCurrentGlyphs.count(); i++ )
			{
				GraphGlyphs& gl = mCurrentGlyphs[ i ];
				if( gl == ggCross || gl == ggHead || gl == ggHeadLeft || gl == ggHeadRight ||
					gl == ggJoin || gl == ggJoinRight || gl == ggJoinLeft )
					gl = ggNotCurrent;
				else if( gl == ggCrossUnused )
					gl = ggUnused;
				else if( gl == ggMergeFork || gl == ggMergeForkRight || gl == ggMergeForkLeft )
					gl = ggCurrent;
			}
		}

		if( didFork )
		{
			for( int i = 0; i < mCurrentGlyphs.count(); i++ )
			{
				GraphGlyphs& gl = mCurrentGlyphs[ i ];
				if( gl == ggCross )
					gl = ggNotCurrent;
				else if( gl == ggTail || gl == ggTailLeft || gl == ggTailRight || gl == ggCrossUnused )
					gl = ggUnused;
				else if( gl == ggMergeFork || gl == ggMergeForkRight || gl == ggMergeForkLeft )
					gl = ggCurrent;
			}

			while( mCurrentGlyphs.last() == ggUnused )
			{
				mCurrentGlyphs.pop_back();
				mNextParent.pop_back();
			}
		}

		if( mCurrentGlyphs[ mCurrentLine ] == ggBranch )
			mCurrentGlyphs[ mCurrentLine ] = ggCurrent;

	}

	mEntries->afterClear();

	qint64 dur = timer.nsecsElapsed();
	double avg = double(dur) / double(mEntries->count());
	QString s = QString::number( avg, 'f' );

	MacGitver::self().log( ltInformation,
						   trUtf8( "Glyphed %1 commits in %2 ns = %3 ns per Commit" )
								.arg( mEntries->count() )
								.arg( dur )
								.arg( s ) );
}
