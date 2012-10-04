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

#ifndef HISTORY_BUILDER_H
#define HISTORY_BUILDER_H

#include <QObject>
#include <QMap>

#include "libGitWrap/ObjectCommit.h"
#include "libGitWrap/Repository.h"
#include "libGitWrap/RevisionWalker.h"

#include "HistoryEntry.h"
#include "HistoryModel.h"

class HistoryBuilder : public QObject
{
	Q_OBJECT
public:
	HistoryBuilder( Git::Repository repo, HistoryModel* model );
	~HistoryBuilder();

public:
	void addHEAD();
	void addAllRefs();

public:
	void start();

private:
	int nextParent( const Git::ObjectId& sha1, int start = 0 );
	int createGlyphSlot( GraphGlyphs glyph, const Git::ObjectId& nextSha1InSlot, int start = 0 );

private:
	void updateReferences();

private:
	Git::Repository		mRepo;
	Git::RevisionWalker	mWalker;
	HistoryModel*		mModel;
	GraphGlyphVector	mCurrentGlyphs;
	int					mCurrentLine;
	Git::ObjectIdList	mNextParent;
};

#endif
