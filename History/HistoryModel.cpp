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

#include <QDebug>
#include <QElapsedTimer>

#include "libMacGitverCore/App/MacGitver.hpp"

#include "libGitWrap/Reference.hpp"

#include "HistoryModel.h"
#include "HistoryEntry.h"
#include "HistoryBuilder.h"

HistoryModel::HistoryModel( const Git::Repository& repo, QObject* parent )
    : QAbstractTableModel( parent )
{
    mRepo = repo;
    mMode = modeSimple;
    mShowRoots = ShowRootHeadOnly;

    mDisplays = 0;

    Q_ASSERT( mRepo.isValid() );
}

HistoryModel::~HistoryModel()
{
    qDeleteAll( mEntries );
}

int HistoryModel::columnMap( int index ) const
{
    switch( mMode )
    {
    case modeSimple:
        switch( index )
        {
        case -1:    return 4;
        case 0:     return colGraph;
        case 1:     return colMessage;
        case 2:     return colAuthor;
        case 3:     return colAuthorDate;
        default:    return -1;
        }

    case modeFull:
        switch( index )
        {
        case -1:    return 7;
        case 0:     return colGraph;
        case 1:     return colMessage;
        case 2:     return colAuthor;
        case 3:     return colAuthorDate;
        case 4:     return colCommitter;
        case 5:     return colCommitterDate;
        case 6:     return colSHA1;
        default:    return -1;
        }

    case modeFancy:
        return 1;
    }

    return -1;
}

int HistoryModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        return 0;
    }
    return mEntries.count();
}

int HistoryModel::columnCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        return 0;
    }
    return columnMap( -1 );
}

HistoryEntry* HistoryModel::at( int row, bool populate ) const
{
    HistoryEntry* e = mEntries[ row ];
    Q_ASSERT( e );

    if( populate && !e->isPopulated() )
    {
        if (!e->isPopulationQueued()) {
            e->setPopulationQueued();

            QMetaObject::invokeMethod( const_cast<HistoryModel*>(this),
                                       "ensurePopulated",
                                       Qt::QueuedConnection,
                                       Q_ARG( int, row ) );
        }

        return NULL;
    }
    return e;
}

HistoryEntry* HistoryModel::indexToEntry( const QModelIndex& index ) const
{
    if( !index.isValid() )
    {
        return NULL;
    }

    return at( index.row() );
}

QVariant HistoryModel::data( const QModelIndex& index, int role ) const
{
    HistoryEntry* e = indexToEntry( index );
    if( !e )
    {
        return QVariant();
    }

    switch( role )
    {
    case Qt::DisplayRole:
        switch( columnMap( index.column() ) )
        {
        case colGraph:          return QVariant();
        case colMessage:        return e->message();
        case colAuthor:         return e->author().fullName();
        case colAuthorDate:     return e->author().when();
        case colCommitter:      return e->committer().fullName();
        case colCommitterDate:  return e->committer().when();
        case colSHA1:           return e->id().toString();
        default:                return QVariant();
        }

    default:
        return QVariant();
    }
}

QVariant HistoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Vertical )
        return QVariant();

    if( role != Qt::DisplayRole )
        return QVariant();

    switch( section )
    {
    case colGraph:          return trUtf8( "Graph" );
    case colMessage:        return trUtf8( "Message" );
    case colAuthor:         return trUtf8( "Author" );
    case colAuthorDate:     return trUtf8( "Author date" );
    case colCommitter:      return trUtf8( "Comitter" );
    case colCommitterDate:  return trUtf8( "Committer date" );
    case colSHA1:           return trUtf8( "SHA1" );
    default:                return QVariant();
    }
}

void HistoryModel::beforeClear()
{
    beginResetModel();
    qDeleteAll(mEntries);
    mEntries.clear();
}

void HistoryModel::afterClear()
{
    scanInlineReferences();
    endResetModel();
}

void HistoryModel::updateRow( int row )
{
    dataChanged( index( row, 0 ), index( row, columnCount() ) );
}

void HistoryModel::beforeAppend()
{
    beginInsertRows( QModelIndex(), mEntries.count(), mEntries.count() );
}

void HistoryModel::afterAppend()
{
    endInsertRows();
}

void HistoryModel::ensurePopulated( int row )
{
    Q_ASSERT( row < mEntries.count() );

    HistoryEntry* e = mEntries[ row ];
    if( !e || e->isPopulated() )
    {
        return;
    }

    Git::Result r;
    Git::ObjectCommit commit = mRepo.lookupCommit( r, e->id() );
    if( r && commit.isValid() )
    {
        e->populate( commit );

        emit dataChanged( index( row, 0 ), index( row, columnMap( -1 ) - 1 ) );
    }
    else
    {
        qDebug() << "Failed to lookup a commit" << r.errorText();
    }
}

void HistoryModel::append( HistoryEntry* entry )
{
    mEntries.append( entry );
}

void HistoryModel::buildHistory()
{
    HistoryBuilder b( mRepo, this );

    switch (mShowRoots) {

    case ShowRootHeadOnly:
        b.addHEAD();
        break;

    case ShowRootLocalBranches:
        b.addBranches(false);
        break;

    case ShowRootAllBranches:
        b.addBranches(true);
        break;

    case ShowRootAllRefs:
        b.addAllRefs();
        break;
    }

    b.start();
}

void HistoryModel::scanInlineReferences()
{
    qint64				dur;
    double				avg;
    QElapsedTimer		timer;
    Git::ResolvedRefs	refs;
    Git::Result			r;
    Git::Reference		refHEAD;
    QHash< Git::ObjectId, HistoryInlineRefs > refsById;

    if( !mRepo.isValid() )
    {
        return;
    }

    // First step: Collect all references.

    refs = mRepo.allResolvedRefs( r );
    refHEAD = mRepo.HEAD( r );
    bool detached = mRepo.isHeadDetached();
    if( !r )
    {
        MacGitver::log( ltError, r.errorText() );
        return;
    }

    timer.start();

    // Second step: Classify the refs and determine a nice display name for them

    foreach( QString ref, refs.keys() )
    {
        HistoryInlineRef inlRef;

        if (mDisplays.testFlag(DisplayLocals) && ref.startsWith(QLatin1String("refs/heads/"))) {
            if (ref.endsWith(QLatin1String("HEAD"))) {
                if (detached) {
                    inlRef.mRefName = trUtf8("<detached head>");
                }
                else {
                    // Skip "HEAD"
                    continue;
                }
            }
            else {
                inlRef.mRefName = ref.mid( strlen( "refs/heads/" ) );
            }
            inlRef.mIsBranch = true;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = false;
            inlRef.mIsStash = false;
            inlRef.mIsCurrent = inlRef.mRefName == refHEAD.shorthand();
        }
        else if (mDisplays.testFlag(DisplayTags) && ref.startsWith(QLatin1String("refs/tags/"))) {
            inlRef.mRefName = ref.mid( strlen( "refs/tags/" ) );
            inlRef.mIsBranch = false;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = true;
            inlRef.mIsCurrent = false;
            inlRef.mIsStash = false;
        }
        else if (mDisplays.testFlag(DisplayRemotes) &&
                 ref.startsWith(QLatin1String("refs/remotes/"))) {

            if (ref.endsWith( QLatin1String("HEAD"))) {
                continue; // Skip "HEAD"
            }
            inlRef.mRefName = ref.mid( strlen( "refs/remotes/" ) );
            inlRef.mIsBranch = true;
            inlRef.mIsRemote = true;
            inlRef.mIsTag = false;
            inlRef.mIsCurrent = false;
            inlRef.mIsStash = false;
        }
        else if (ref == QLatin1String("refs/stash")) {
            inlRef.mRefName = trUtf8( "<recent stash>" );
            inlRef.mIsBranch = false;
            inlRef.mIsCurrent = true;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = false;
            inlRef.mIsStash = true;
        }
        else {
            qDebug() << "HistoryModel::scanInlineReferences => Unhandled ref:" << ref;
            continue;
        }

        if (!refsById.contains(refs[ref])) {
            refsById.insert(refs[ref], HistoryInlineRefs());
        }

        refsById[refs[ref]].append(inlRef);
    }

    // Third step: Update the commitlist and mix it with the inline Refs we just found.

    for( int i = 0; i < mEntries.count(); i++ )
    {
        HistoryEntry* e = mEntries[i];
        Q_ASSERT( e );

        HistoryInlineRefs newRefs = refsById.value( e->id() );
        HistoryInlineRefs oldRefs = e->refs();

        if( !newRefs.count() )
        {
            if( !oldRefs.count() )
            {
                continue;
            }
            e->setInlineRefs( newRefs );
            updateRow( i );
        }
        else
        {
            if( oldRefs.count() != newRefs.count() )
            {
                e->setInlineRefs( newRefs );
                updateRow( i );
                continue;
            }

            int diffs = newRefs.count();
            for( int j = 0; j < newRefs.count(); j++ )
            {
                QString newRef = newRefs.at( j ).mRefName;
                for( int k = 0; k < oldRefs.count(); k++ )
                {
                    if( oldRefs.at( k ).mRefName == newRef )
                    {
                        diffs--;
                        break;
                    }
                }
            }

            if( diffs )
            {
                e->setInlineRefs( newRefs );
                updateRow( i );
            }
        }
    }

    dur = timer.nsecsElapsed();
    avg = double( dur ) / double( refs.count() );
    MacGitver::log( ltInformation,
                    trUtf8( "Found and peeled %1 refs in %2 ns = %3 ns per ref." )
                        .arg( refs.count() )
                        .arg( dur )
                        .arg( avg, 10, 'f', 2 ) );
}

void HistoryModel::changeDisplays(InlineRefDisplays displays, bool activate)
{
    InlineRefDisplays old = mDisplays;

    if (activate)
        mDisplays |= displays;
    else
        mDisplays &= ~displays;

    if (mDisplays != old)
        scanInlineReferences();
}

void HistoryModel::setShowRoots(Roots roots)
{
    if (mShowRoots != roots ) {
        mShowRoots = roots;
        buildHistory();
    }
}
