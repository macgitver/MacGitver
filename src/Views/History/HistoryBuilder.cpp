
#include "Libs/Git/ObjectId.h"
#include "Libs/Git/Reference.h"

#include "Views/History/HistoryBuilder.h"
#include "Views/History/HistoryEntry.h"

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
