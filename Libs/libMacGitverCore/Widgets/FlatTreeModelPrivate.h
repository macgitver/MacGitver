/*
 * MacGitver
 * Copyright (C) 2012-2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@cunz-rad.com>
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

#ifndef MGV_FLAT_TREE_MODEL_PRIVATE_H
#define MGV_FLAT_TREE_MODEL_PRIVATE_H

#include <QIcon>
#include <QVariant>

#include "FlatTreeModel.h"

struct FlatTreeModelEntry
{
    QIcon               mIcon;
    QString             mText;
    QString             mPath;
    QVariant            mUserData;
    bool                mIsHeader;
    int                 mIndex;
    int                 mLevel;
    FlatTreeModelEntry* mParent;
    FlatTreeModelEntry* mFirstChild;
    FlatTreeModelEntry* mNextSibling;
};

class FlatTreeModelPrivate
{
public:
    FlatTreeModelPrivate( FlatTreeModel* owner );
    ~FlatTreeModelPrivate();

public:
    FlatTreeModelEntry* entryByPath( const QString& path );
    FlatTreeModelEntry* entryByPath( const QString& path, QStringList& remainder );
    FlatTreeModelEntry* subEntry( FlatTreeModelEntry* parent, QStringList& remainder );
    int updateIndicies( FlatTreeModelEntry* cur, int& currentIndex,
                        int level, FlatTreeModelEntry* search );

    bool addEntry( const QString& path );
    bool removeEntry( const QString& path );

    bool rawRemove( FlatTreeModelEntry* entry );

public:
    FlatTreeModel*                  mOwner;
    QChar                           mSeparator;
    QIcon                           mDefaultHeaderIcon;
    QIcon                           mDefaultEntryIcon;
    QList< FlatTreeModelEntry* >    mEntries;
    FlatTreeModelEntry              mRoot;
};

#endif
