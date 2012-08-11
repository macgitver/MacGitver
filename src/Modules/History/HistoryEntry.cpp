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


HistoryEntry::HistoryEntry( const Git::ObjectCommit& commit )
{
	mCommiter = commit.committer();
	mAuthor = commit.author();
	mSha1 = commit.id();
	mCommitMessage = commit.shortMessage();
}

QString HistoryEntry::message() const
{
	return mCommitMessage;
}

const Git::Signature& HistoryEntry::committer() const
{
	return mCommiter;
}

const Git::Signature& HistoryEntry::author() const
{
	return mAuthor;
}

const Git::ObjectId& HistoryEntry::id() const
{
	return mSha1;
}

void HistoryEntry::setGlyphs( const GraphGlyphVector& glyphs )
{
	mGlyphs = glyphs;
}

const GraphGlyphVector&	HistoryEntry::glyphs() const
{
	return mGlyphs;
}

void HistoryEntry::setInlineRefs( const HistoryInlineRefs& refs )
{
	mRefs = refs;
}

const HistoryInlineRefs& HistoryEntry::refs() const
{
	return mRefs;
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
//	emit beforeAppend();

	mEntries.append( entry );

//	emit afterAppend();
}

void HistoryEntries::clear()
{
	emit beforeClear();

	qDeleteAll( mEntries );
	mEntries.clear();

	emit afterClear();
}
