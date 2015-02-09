/*
 * MacGitver
 * Copyright (C) 2015 The MacGitver-Developers <dev@macgitver.org>
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
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Ref.hpp"

#include "libGitWrap/Reference.hpp"

#include "HistoryModel.h"
#include "HistoryBuilder.h"

HistoryModel::HistoryModel( const Git::Repository& repo, QObject* parent )
    : QAbstractTableModel( parent )
{
    mRepo = repo;
    Q_ASSERT( mRepo.isValid() );

    mMode = modeSimple;
    mShowRoots = ShowRootHeadOnly;

    mDisplays = 0;

    RM::RepoMan &rm = MacGitver::repoMan();
    connect( &rm, SIGNAL(refCreated(RM::Repo*,RM::Ref*)), this, SLOT(onRefCreated(RM::Repo*,RM::Ref*)) );
    connect( &rm, SIGNAL(refAboutToBeDeleted(RM::Repo*,RM::Ref*)), this, SLOT(onRefDestroyed(RM::Repo*,RM::Ref*)) );
    connect( &rm, SIGNAL(refMoved(RM::Repo*,RM::Ref*)), this, SLOT(onRefMoved(RM::Repo*,RM::Ref*)) );
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
        case -1:    return 5;
        case 0:     return colGraph;
        case 1:     return colMessage;
        case 2:     return colAuthor;
        case 3:     return colAuthorMail;
        case 4:     return colAuthorDate;
        default:    return -1;
        }

    case modeFull:
        switch( index )
        {
        case -1:    return 8;
        case 0:     return colGraph;
        case 1:     return colMessage;
        case 2:     return colAuthor;
        case 3:     return colAuthorDate;
        case 4:     return colCommitter;
        case 5:     return colCommitterMail;
        case 6:     return colCommitterDate;
        case 7:     return colSHA1;
        default:    return -1;
        }

    case modeFancy:
        return 1;
    }

    return -1;
}

QModelIndex HistoryModel::indexByObjectId(const Git::ObjectId& id) const
{
    for ( int row=0; row <  mEntries.count(); ++row ) {
        const HistoryEntry* e = at( row );

        if ( e && (e->id() == id) ) {
            return index( row, 0, QModelIndex() );
        }
    }

    return QModelIndex();
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
    Q_ASSERT( !parent.isValid() );
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
        case colAuthor:         return e->author().name();
        case colAuthorMail:     return e->author().email();
        case colAuthorDate:     return e->author().when();
        case colCommitter:      return e->committer().name();
        case colCommitterMail:  return e->committer().email();
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
    case colAuthorMail:     return trUtf8( "Author mail" );
    case colAuthorDate:     return trUtf8( "Author date" );
    case colCommitter:      return trUtf8( "Comitter" );
    case colCommitterMail:  return trUtf8( "Committer mail" );
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

void HistoryModel::updateRows(int firstRow, int lastRow)
{
    emit dataChanged( index( firstRow, 0 ), index( lastRow, columnCount() - 1 ) );
}

void HistoryModel::beforeAppend()
{
    beginInsertRows( QModelIndex(), mEntries.count(), mEntries.count() );
}

void HistoryModel::afterAppend()
{
    endInsertRows();
}

/**
 * @internal
 * @see RM::EventInterface
 */
///@{
void HistoryModel::onRefCreated(RM::Repo* repo, RM::Ref* ref)
{
    Q_UNUSED( ref )

    if ( !repo || (repo->gitLoadedRepo() != mRepo) ) {
        return;
    }

    scanInlineReferences();
}

void HistoryModel::onRefDestroyed(RM::Repo* repo, RM::Ref* ref)
{
    Q_UNUSED( ref )

    if ( !repo || (repo->gitLoadedRepo() != mRepo) ) {
        return;
    }

    scanInlineReferences();
}

void HistoryModel::onRefMoved(RM::Repo* repo, RM::Ref* ref)
{
    Q_UNUSED( ref );

    if ( !repo || (repo->gitLoadedRepo() != mRepo) ) {
        return;
    }

    scanInlineReferences();
}
///@}

void HistoryModel::ensurePopulated( int row )
{
    HistoryEntry* e = mEntries[ row ];
    if( !e || e->isPopulated() )
    {
        return;
    }

    Git::Result r;
    Git::Commit commit = mRepo.lookupCommit( r, e->id() );
    if( r && commit.isValid() )
    {
        e->populate( commit );

        updateRows( row, row );
    }
    else
    {
        qDebug() << "Failed to lookup a commit: " << r.errorText();
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
    qint64              dur;
    double              avg;
    Git::ResolvedRefs   refs;
    Git::Result         r;
    Git::Reference      refHEAD;
    QHash< Git::ObjectId, HistoryInlineRefs > refsById;

    // First step: Collect all references.

    refs = mRepo.allResolvedRefs( r );
    refHEAD = mRepo.HEAD( r );
    if( !r )
    {
        MacGitver::log(Log::Error, r.errorText());
        return;
    }

    bool detached = mRepo.isHeadDetached();
    if ( detached ) {
        // append HEAD when detached
        refs[ QStringLiteral("< DETACHED >") ] = refHEAD.resolveToObjectId( r );
    }

    QElapsedTimer   stopwatch;
    stopwatch.start();

    // Second step: Classify the refs and determine a nice display name for them

    foreach( QString ref, refs.keys() )
    {
        HistoryInlineRef inlRef;
        inlRef.mIsDetached = detached;

        if ( mDisplays.testFlag(DisplayLocals) && ref.startsWith(QStringLiteral("refs/heads/")) ) {
            inlRef.mRefName = ref.mid( strlen( "refs/heads/" ) );
            inlRef.mIsBranch = true;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = false;
            inlRef.mIsStash = false;
            inlRef.mIsCurrent = !detached ? (inlRef.mRefName == refHEAD.shorthand()) : false;
        }
        else if (mDisplays.testFlag( DisplayTags ) && ref.startsWith( QStringLiteral("refs/tags/") ) ) {
            inlRef.mRefName = ref.mid( strlen( "refs/tags/" ) );
            inlRef.mIsBranch = false;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = true;
            inlRef.mIsCurrent = false;
            inlRef.mIsStash = false;
        }
        else if (mDisplays.testFlag(DisplayRemotes) &&
                 ref.startsWith( QStringLiteral("refs/remotes/")) ) {

            if (ref.endsWith( QStringLiteral("HEAD"))) {
                continue; // Skip "HEAD"
            }
            inlRef.mRefName = ref.mid( strlen( "refs/remotes/" ) );
            inlRef.mIsBranch = true;
            inlRef.mIsRemote = true;
            inlRef.mIsTag = false;
            inlRef.mIsCurrent = false;
            inlRef.mIsStash = false;
        }
        else if (ref == QStringLiteral("refs/stash")) {
            inlRef.mRefName = tr( "<recent stash>" );
            inlRef.mIsBranch = false;
            inlRef.mIsCurrent = true;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = false;
            inlRef.mIsStash = true;
        }
        else if ( detached && ref == QStringLiteral("< DETACHED >") ) {
            inlRef.mRefName = tr("< DETACHED >");
            inlRef.mIsBranch = false;
            inlRef.mIsCurrent = false;
            inlRef.mIsRemote = false;
            inlRef.mIsTag = false;
            inlRef.mIsStash = false;
        }
        else {
            // qDebug() << "HistoryModel::scanInlineReferences => Unhandled ref:" << ref;
            continue;
        }

        Git::ObjectId oid = refs[ref];
        if ( !refsById.contains( oid ) ) {
            refsById.insert( oid, HistoryInlineRefs() );
        }

        refsById[ oid ].append( inlRef );
    }

    // Third step: Update the commitlist and mix it with the inline Refs we just found.

    updateInlineRefs( refsById );

    dur = stopwatch.nsecsElapsed();
    avg = double( dur ) / double( refs.count() );
    MacGitver::log(Log::Information,
                   trUtf8("Found and resolved %1 refs in %2 ns = %3 ns per ref.")
                        .arg(refs.count())
                        .arg(dur)
                        .arg(avg, 10, 'f', 2));
}

void HistoryModel::updateInlineRefs(const QHash<Git::ObjectId, HistoryInlineRefs>& refsById)
{
    for( int i = 0; i < mEntries.count(); i++ )
    {
        HistoryEntry* e = mEntries[i];
        Q_ASSERT( e );

        HistoryInlineRefs newRefs = refsById.value( e->id() );
        HistoryInlineRefs oldRefs = e->refs();

        // sort newRefs: no need to sort oldRefs as it is already sorted)
        std::sort( newRefs.begin(), newRefs.end() );

        if( oldRefs.count() != newRefs.count() )
        {
            e->setInlineRefs( newRefs );
            updateRows( i, i );
            continue;
        }

        for ( int j = 0; j < newRefs.count(); j++ ) {
            const HistoryInlineRef& newRef = newRefs[j];
            const HistoryInlineRef& oldRef = oldRefs[j];
            if ( newRef != oldRef ) {
                e->setInlineRefs( newRefs );
                updateRows( i, i );
                break;
            }
        }
    }
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
