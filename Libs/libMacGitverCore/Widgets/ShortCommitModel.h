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

#pragma once

#include "libMacGitverCore/MacGitverApi.hpp"

#include <QAbstractItemModel>

#include "libGitWrap/Commit.hpp"

class MGV_CORE_API ShortCommitModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ShortCommitModel( QObject* parent = 0 );
    ~ShortCommitModel();

public:
    void clear();
    void setCommitList( const Git::CommitList& list );
    bool setCommitList( Git::Repository repo, const Git::ObjectIdList& list );

public:
    Git::Commit indexToCommit( const QModelIndex& index ) const;

public:
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

private:
    Git::CommitList mCommitList;
};
