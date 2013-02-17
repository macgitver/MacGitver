/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_DIFF_GIT_PATCH_CONSUMER_H
#define MGV_DIFF_GIT_PATCH_CONSUMER_H

#include "libMacGitverCore/MacGitverApi.hpp"

#include "libGitWrap/PatchConsumer.hpp"

#include "libDiffViews/Model/Patch.hpp"
#include "libDiffViews/Model/TextFilePatch.hpp"

class MGV_CORE_API GitPatchConsumer : public Git::PatchConsumer
{
public:
    GitPatchConsumer();
    virtual ~GitPatchConsumer();

private:
    virtual bool startFileChange( const Git::ChangeListEntry &entry );

    virtual bool startHunkChange( int newStart, int newLines, int oldStart, int oldLines,
                                  const QString& header );

    virtual bool appendContext( const QString& content );
    virtual bool appendAddition( const QString& content );
    virtual bool appendDeletion( const QString& content );

public:
    DiffViews::Patch::Ptr patch() const;

private:
    DiffViews::Patch::Ptr       mPatch;
    DiffViews::FilePatch::Ptr   mCurFile;
    DiffViews::Hunk::Ptr        mCurHunk;
    DiffViews::HunkPart::Ptr    mCurDiff;

    int                 mRemainsOld, mCurOld;
    int                 mRemainsNew, mCurNew;
    enum { None, Context, Add, Del, Change }
                        mCurType;
};

#endif
