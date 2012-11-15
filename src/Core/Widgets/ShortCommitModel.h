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

#ifndef MGV_SHORT_COMMIT_MODEL_H
#define MGV_SHORT_COMMIT_MODEL_H

#include "MacGitverApi.hpp"

#include <QAbstractItemModel>

#include "libGitWrap/ObjectCommit.h"

class MGV_CORE_API ShortCommitModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ShortCommitModel( QObject* parent = 0 );
    ~ShortCommitModel();

public:
    void clear();
    void setCommitList( const QList< Git::ObjectCommit >& list );
    bool setCommitList( Git::Repository repo, const QList< Git::ObjectId >& list );

public:
    Git::ObjectCommit indexToCommit( const QModelIndex& index ) const;

public:
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

private:
    QList< Git::ObjectCommit > mCommitList;
};

#endif
