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

#include "HistoryEntry.h"

HistoryGraphDrawInfos::HistoryGraphDrawInfos()
{
	mNumEntries = 0;
}

HistoryGraphDrawInfos::~HistoryGraphDrawInfos()
{
	if( mNumEntries > 8 )
	{
		delete[] mData;
	}
}

HistoryGraphDrawInfo HistoryGraphDrawInfos::at( unsigned char idx ) const
{
	if( mNumEntries > 8 )
	{
		return mData[ idx ];
	}
	else
	{
		return mOwnData[ idx ];
	}
}

unsigned char HistoryGraphDrawInfos::count() const
{
	return mNumEntries;
}

void HistoryGraphDrawInfos::add( HistoryGraphDrawInfo di )
{
	if( mNumEntries < 8 )
	{
		mOwnData[ mNumEntries++ ] = di;
	}
	else if( mNumEntries > 8 && mNumEntries < mAlloced )
	{
		mData[ mNumEntries++ ] = di;
	}
	else if( mNumEntries > 8 )
	{
		mAlloced += 8;
		mData = (HistoryGraphDrawInfo*) realloc( mData, sizeof( HistoryGraphDrawInfo ) * mAlloced );
		mData[ mNumEntries++ ] = di;
	}
	else
	{
		HistoryGraphDrawInfo* hgdi = (HistoryGraphDrawInfo*)
									 malloc( sizeof( HistoryGraphDrawInfo ) * 16 );
		memcpy( hgdi, mOwnData, sizeof( HistoryGraphDrawInfo ) * 8 );

		mData = hgdi;
		mAlloced = 16;

		mData[ mNumEntries++ ] = di;
	}
}

//-----------

HistoryEntry::HistoryEntry( const Git::ObjectCommit& commit, unsigned char slot )
{
	mSlot = slot;
	mCommiter = commit.committer();
	mAuthor = commit.author();
	mSha1 = commit.id();
	mCommitMessage = commit.shortMessage();
}

QString HistoryEntry::message() const
{
	return mCommitMessage;
}

Git::Signature HistoryEntry::committer() const
{
	return mCommiter;
}

Git::Signature HistoryEntry::author() const
{
	return mAuthor;
}

Git::ObjectId HistoryEntry::id() const
{
	return mSha1;
}

unsigned char HistoryEntry::slot() const
{
	return mSlot;
}

//-----------

HistoryEntries::HistoryEntries()
{
}

HistoryEntries::~HistoryEntries()
{
	qDeleteAll( mEntries );
}

int HistoryEntries::count() const
{
	return mEntries.count();
}

HistoryEntry* HistoryEntries::at( int index )
{
	return mEntries.at( index );
}

void HistoryEntries::append( HistoryEntry* entry )
{
	emit beforeAppend();

	mEntries.append( entry );

	emit afterAppend();
}

void HistoryEntries::clear()
{
	emit beforeClear();

	qDeleteAll( mEntries );
	mEntries.clear();

	emit afterClear();
}
