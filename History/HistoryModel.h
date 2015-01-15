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

#ifndef HISTORY_MODEL_H
#define HISTORY_MODEL_H

#include <QVector>
#include <QAbstractTableModel>

#include "libGitWrap/Repository.hpp"

class HistoryEntry;

class HistoryModel : public QAbstractTableModel
{
    friend class HistoryBuilder;
    Q_OBJECT
public:
    enum Columns
    {
        colGraph,
        colMessage,
        colAuthor,
        colAuthorMail,
        colAuthorDate,
        colCommitter,
        colCommitterMail,
        colCommitterDate,
        colSHA1
    };

    enum Modes
    {
        modeFull,
        modeSimple,
        modeFancy
    };

    enum Roots {
        ShowRootAllBranches,
        ShowRootHeadOnly,
        ShowRootLocalBranches,
        ShowRootAllRefs
    };

    enum InlineRefDisplay {
        DisplayLocals       = 1 << 0,
        DisplayRemotes      = 1 << 1,
        DisplayTags         = 1 << 2
    };

    typedef QFlags< InlineRefDisplay > InlineRefDisplays;

public:
    HistoryModel( const Git::Repository& repo, QObject* parent = NULL );
    ~HistoryModel();

public:
    int columnMap( int index ) const;

public:
    QModelIndex indexByObjectId( const Git::ObjectId& id ) const;

    HistoryEntry* indexToEntry( const QModelIndex& index ) const;
    HistoryEntry* at( int row, bool populate = true ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

public:
    void setShowRoots( Roots roots );
    void changeDisplays(InlineRefDisplays displays, bool activate);
    void append( HistoryEntry* entry );
    void buildHistory();

private:
    void updateRows( int firstRow, int lastRow );
    void scanInlineReferences();

public slots:
    void ensurePopulated( int row );

private slots:
    void beforeClear();
    void afterClear();
    void beforeAppend();
    void afterAppend();

private:
    InlineRefDisplays           mDisplays;
    Git::Repository             mRepo;
    Modes                       mMode;
    QVector< HistoryEntry* >    mEntries;
    Roots                       mShowRoots;
};

#endif
