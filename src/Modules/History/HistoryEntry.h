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

#ifndef HISTORY_ENTRY_H
#define HISTORY_ENTRY_H

#include "GitWrap/Signature.h"
#include "GitWrap/ObjectId.h"
#include "GitWrap/ObjectCommit.h"

struct HistoryGraphDrawInfo
{
	bool			endsUpwards		: 1;
	bool			endsDownwards	: 1;
	bool			startsMiddle	: 1;
	unsigned int	startColumn		: 10;
	unsigned int	endColumn		: 10;
	unsigned int	colorIndex		: 5;
};

class HistoryGraphDrawInfos
{
public:
	HistoryGraphDrawInfos();
	~HistoryGraphDrawInfos();

public:
	HistoryGraphDrawInfo at( unsigned char idx ) const;
	unsigned char count() const;
	void add( HistoryGraphDrawInfo di );

private:
	unsigned char		mNumEntries;
	union
	{
		struct
		{
			HistoryGraphDrawInfo*	mData;
			unsigned char			mAlloced;
		};
		HistoryGraphDrawInfo mOwnData[ 8 ];
	};
};

class HistoryEntry
{
public:
	HistoryEntry( const Git::ObjectCommit& commit, unsigned char slot );

public:
	QString message() const;
	Git::Signature committer() const;
	Git::Signature author() const;
	Git::ObjectId id() const;
	unsigned char slot() const;

	HistoryGraphDrawInfos& drawInfos(){ return mDrawInfos; }
	void addDrawInfo( HistoryGraphDrawInfo& di ){ mDrawInfos.add( di ); }

private:
	QString					mCommitMessage;
	Git::ObjectId			mSha1;
	Git::Signature			mCommiter;
	Git::Signature			mAuthor;
	unsigned char			mSlot;
	HistoryGraphDrawInfos	mDrawInfos;
};

class HistoryEntries : public QObject
{
	Q_OBJECT
public:
	HistoryEntries();
	~HistoryEntries();

public:
	void clear();
	int count() const;
	HistoryEntry* at( int index );
	void append( HistoryEntry* entry );

signals:
	void beforeClear();
	void afterClear();
	void beforeAppend();
	void afterAppend();

private:
	QList< HistoryEntry* > mEntries;
};

#endif
