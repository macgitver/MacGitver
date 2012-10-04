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

#include "libGitWrap/Result.h"

#include "MacGitver/MacGitver.h"

#include "HistoryEntry.h"

HistoryEntry::HistoryEntry( const Git::ObjectId& sha1 )
	: mSha1( sha1 )
	, mPopulated( false )
{
}

void HistoryEntry::populate( const Git::ObjectCommit& commit )
{
	Q_ASSERT( !mPopulated );

	Git::Result r;

	mCommiter = commit.committer( r );
	mAuthor = commit.author( r );
	mCommitMessage = commit.shortMessage( r );

	mPopulated = r;

	if( !mPopulated )
	{
		MacGitver::self().log( ltError, r, "Populating a HistoryEntry" );
	}
}

bool HistoryEntry::isPopulated() const
{
	return mPopulated;
}

QString HistoryEntry::message() const
{
	Q_ASSERT( mPopulated );
	return mCommitMessage;
}

const Git::Signature& HistoryEntry::committer() const
{
	Q_ASSERT( mPopulated );
	return mCommiter;
}

const Git::Signature& HistoryEntry::author() const
{
	Q_ASSERT( mPopulated );
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
