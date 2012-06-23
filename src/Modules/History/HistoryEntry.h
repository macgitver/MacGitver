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

enum GraphGlyphs
{
	ggUnused,

	ggCurrent,
	ggNotCurrent,

	ggMergeFork,
	ggMergeForkRight,
	ggMergeForkLeft,

	ggJoin,
	ggJoinLeft,
	ggJoinRight,

	ggHead,
	ggHeadLeft,
	ggHeadRight,

	ggTail,
	ggTailLeft,
	ggTailRight,

	ggCross,
	ggCrossUnused,

	ggInitial,
	ggBranch
};

typedef QVector< GraphGlyphs > GraphGlyphVector;

class HistoryEntry
{
public:
	HistoryEntry( const Git::ObjectCommit& commit );

public:
	QString message() const;
	const Git::Signature&	committer() const;
	const Git::Signature&	author() const;
	const Git::ObjectId&	id() const;
	const GraphGlyphVector&	glyphs() const;

public:
	void setGlyphs( const GraphGlyphVector& glyphs );

private:
	QString				mCommitMessage;
	Git::ObjectId		mSha1;
	Git::Signature		mCommiter;
	Git::Signature		mAuthor;
	GraphGlyphVector	mGlyphs;
};

class HistoryBuilder;

class HistoryEntries : public QObject
{
	friend class HistoryBuilder;
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
