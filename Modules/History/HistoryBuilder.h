/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
 * (C) Cunz RaD Ltd.
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

#include "libGitWrap/Commit.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/RevisionWalker.hpp"

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
    void addBranches(bool includeRemotes);
    void addAllRefs();

public:
    void start();

private:
    int nextParent( const Git::ObjectId& sha1, int start = 0 );
    int createGlyphSlot( GraphGlyphs glyph, const Git::ObjectId& nextSha1InSlot, int start = 0 );

private:
    Git::Repository     mRepo;
    Git::RevisionWalker mWalker;
    HistoryModel*       mModel;
    GraphGlyphVector    mCurrentGlyphs;
    int                 mCurrentLine;
    Git::ObjectIdList   mNextParent;
};

#endif
