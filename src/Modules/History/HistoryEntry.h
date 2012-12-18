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

#include "libGitWrap/Signature.hpp"
#include "libGitWrap/ObjectId.hpp"
#include "libGitWrap/ObjectCommit.hpp"

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

struct HistoryInlineRef
{
    QString     mRefName;
    bool        mIsBranch   : 1;
    bool        mIsCurrent  : 1;
    bool        mIsRemote   : 1;
    bool        mIsTag      : 1;
    bool        mIsStash    : 1;
};

typedef QVector< HistoryInlineRef > HistoryInlineRefs;

class HistoryEntry
{
public:
    HistoryEntry( const Git::ObjectId& sha1 );

public:
    bool                        isPopulated() const;

    QString                     message() const;
    const Git::Signature&       committer() const;
    const Git::Signature&       author() const;
    const Git::ObjectId&        id() const;
    const GraphGlyphVector&     glyphs() const;
    const HistoryInlineRefs&    refs() const;

public:
    void populate( const Git::ObjectCommit& commit );
    void setGlyphs( const GraphGlyphVector& glyphs );
    void setInlineRefs( const HistoryInlineRefs& refs );

private:
    Git::ObjectId       mSha1;
    bool                mPopulated;

    QString             mCommitMessage;
    Git::Signature      mCommiter;
    Git::Signature      mAuthor;

    GraphGlyphVector    mGlyphs;
    HistoryInlineRefs   mRefs;
};

#endif
