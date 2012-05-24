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
		mWalker.pushRef( s.toAscii() );
	}
}

void HistoryBuilder::start()
{
	Git::ObjectId current;
	Git::ObjectCommit prevCommit;
	Git::ObjectCommit curCommit;
	int lastSlot = -1;
	int slot = -1;
	HistoryEntry* lastEntry = 0;
	HistoryEntry* curEntry = 0;

	QList< Git::ObjectCommit > mLastSlotEntries;

	while( mWalker.next( current ) )
	{
		lastEntry = curEntry;
		Q_UNUSED( lastEntry );
		lastSlot = slot;
		prevCommit = curCommit;
		curCommit = mRepo.lookupCommit( current );

		slot = -1;
		for( int i = 0; i < mLastSlotEntries.count(); i++ )
		{
			Git::ObjectCommit that = mLastSlotEntries[ i ];
			if( that.numParentCommits() > 0 && curCommit == that.parentCommit( 0 )  )
			{
				slot = i;
				break;
			}
		}

		if( slot == -1 )
		{
			int i = 0;
			if( lastSlot >= 0 )
				i = lastSlot;

			for( ; i < mLastSlotEntries.count(); i++ )
			{
				if( !mLastSlotEntries[ i ].isValid() )
				{
					slot = i;
					break;
				}
			}
		}

		if( slot == -1 )
		{
			slot = mLastSlotEntries.count();
			mLastSlotEntries.append( Git::ObjectCommit() );
		}

		curEntry = new HistoryEntry( curCommit, slot );
		mEntries->append( curEntry );

		for( int i = 0; i < mLastSlotEntries.count(); i++ )
		{
			if( i > slot && mLastSlotEntries[ i ].isValid() &&
					curCommit.isParentOf( mLastSlotEntries[ i ] ) )
			{
				HistoryGraphDrawInfo di;
				di.endsDownwards = false;
				di.endsUpwards = true;
				di.startsMiddle = true;
				di.endColumn = i;
				di.startColumn = slot;
				curEntry->addDrawInfo( di );
				mLastSlotEntries[ i ] = Git::ObjectCommit();
			}
			else if( i == slot || mLastSlotEntries[ i ].isValid() )
			{
				HistoryGraphDrawInfo di;
				di.endsDownwards = di.endsUpwards = true;
				di.endColumn = di.startColumn = i;
				curEntry->addDrawInfo( di );
			}
		}

		mLastSlotEntries[ slot ] = curCommit;
	}
}
