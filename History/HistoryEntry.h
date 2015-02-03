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
    bool        mIsDetached : 1;

    bool operator ==(const HistoryInlineRef& other) const
    {
        if ( this == &other ) {
            return true;
        }

        return  mRefName    == other.mRefName       &&
                mIsBranch   == other.mIsBranch      &&
                mIsCurrent  == other.mIsCurrent     &&
                mIsRemote   == other.mIsRemote      &&
                mIsTag      == other.mIsTag         &&
                mIsStash    == other.mIsStash       &&
                mIsDetached == other.mIsDetached    ;
    }

    inline bool operator !=(const HistoryInlineRef& other) const {
        return !( *this == other );
    }
};

typedef QList< HistoryInlineRef > HistoryInlineRefs;

struct HistoryInlineRef_LessThan
{
    inline bool detachedHEAD(const HistoryInlineRef& ref) const
    {
        return ref.mIsDetached && !(ref.mIsTag || ref.mIsBranch || ref.mIsStash);
    }

    inline bool nameLessThan(const HistoryInlineRef& a, const HistoryInlineRef& b) const
    {
        return a.mRefName < b.mRefName;
    }

    bool operator ()(const HistoryInlineRef& a, const HistoryInlineRef& b) const
    {
        // sort order:
        // - tag
        // - detached HEAD
        // - current branch
        // - local branch
        // - remote branch
        // - stash
        // - Everything else is sorted by mRefName

        if ( a.mIsTag ) {
            // tags have highest priority
            return b.mIsTag ? nameLessThan(a, b) : true;
        }

        if ( detachedHEAD( a ) )
        {
            // a detached HEAD comes right after tag ? -> There can only be one!
            return !b.mIsTag;
        }

        if ( a.mIsBranch )
        {
            if ( b.mIsTag || b.mIsCurrent || detachedHEAD( b ) )
                return false;

            if ( a.mIsCurrent ) {
                // a is the current branch
                return true;
            }

            return ( a.mIsRemote == b.mIsRemote ) ? nameLessThan( a, b ) : !a.mIsRemote;
        }

        if ( a.mIsStash ) {
            return b.mIsStash ? nameLessThan(a, b) : !(b.mIsTag || b.mIsBranch || detachedHEAD( b ));
        }

        return !(b.mIsTag || b.mIsBranch || b.mIsStash || detachedHEAD( b )) ? nameLessThan( a, b ) : false;
    }
};


class HistoryEntry
{
public:
    HistoryEntry( const Git::ObjectId& sha1 );

public:
    bool                        isPopulated() const;
    bool                        isPopulationQueued() const;
    void                        setPopulationQueued();

    QString                     message() const;
    const Git::Signature&       committer() const;
    const Git::Signature&       author() const;
    const Git::ObjectId&        id() const;
    const GraphGlyphVector&     glyphs() const;
    const HistoryInlineRefs&    refs() const;

public:
    void populate( const Git::Commit& commit );
    void setGlyphs( const GraphGlyphVector& glyphs );
    void setInlineRefs( const HistoryInlineRefs& refs );

private:
    Git::ObjectId       mSha1;
    bool                mPopulated : 1;
    bool                mPopulationQueued : 1;

    QString             mCommitMessage;
    Git::Signature      mCommiter;
    Git::Signature      mAuthor;

    GraphGlyphVector    mGlyphs;
    HistoryInlineRefs   mRefs;
};

#endif
