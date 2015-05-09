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

#include <QStringList>

#include "FlatTreeModelPrivate.h"

FlatTreeModelPrivate::FlatTreeModelPrivate( FlatTreeModel* owner )
{
    mOwner = owner;
    mSeparator = QChar( L'/' );
    mRoot.mParent = mRoot.mFirstChild = mRoot.mNextSibling = NULL;
}

FlatTreeModelEntry* FlatTreeModelPrivate::entryByPath( const QString& path )
{
    QStringList remainder;
    FlatTreeModelEntry* entry = entryByPath( path, remainder );
    return remainder.count() ? NULL : entry;
}

FlatTreeModelEntry* FlatTreeModelPrivate::entryByPath( const QString& path, QStringList& remainder )
{
    remainder = path.split( mSeparator );
    return subEntry( &mRoot, remainder );
}

FlatTreeModelEntry* FlatTreeModelPrivate::subEntry( FlatTreeModelEntry* parent,
                                                    QStringList& remainder )
{
    Q_ASSERT( parent );

    if( remainder.count() == 0 )
    {
        return parent;
    }

    QString me = remainder.at( 0 );

    FlatTreeModelEntry* cur = parent->mFirstChild;
    while( cur )
    {
        if( cur->mText == me )
        {
            remainder.removeFirst();
            return subEntry( cur, remainder );
        }

        cur = cur->mNextSibling;
    }

    return parent;
}

int FlatTreeModelPrivate::updateIndicies( FlatTreeModelEntry* cur, int& currentIndex,
                                          int level, FlatTreeModelEntry* search )
{
    int found = -1;
    while( cur )
    {
        cur->mIndex = currentIndex++;
        cur->mLevel = level;
        if( cur == search )
            found = cur->mIndex;

        if( cur->mFirstChild )
        {
            int i = updateIndicies( cur->mFirstChild, currentIndex, level + 1, search );
            if( found == -1 )
                found = i;
        }

        cur = cur->mNextSibling;
    }

    return found;
}

bool FlatTreeModelPrivate::addEntry( const QString& path )
{
    QStringList remainder = path.split( mSeparator );
    FlatTreeModelEntry* cur = subEntry( &mRoot, remainder );

    if( remainder.count() == 0 )
    {
        return false;
    }

    while( remainder.count() )
    {
        FlatTreeModelEntry* newEntry = new FlatTreeModelEntry;
        newEntry->mFirstChild = NULL;
        newEntry->mParent = cur;

        if( cur->mFirstChild )
        {
            FlatTreeModelEntry** ppar = &cur->mFirstChild;
            while( *ppar && (*ppar)->mText < remainder.at(0) )
                ppar = &(*ppar)->mNextSibling;
            newEntry->mNextSibling = *ppar;
            *ppar = newEntry;
        }
        else
        {
            newEntry->mNextSibling = cur->mFirstChild;
            cur->mFirstChild = newEntry;
        }

        newEntry->mIsHeader = remainder.count() > 1;
        newEntry->mText = remainder.takeFirst();

        int curIdx = -1;
        int idx = updateIndicies( &mRoot, curIdx, 0, newEntry );
        mOwner->beginInsertRows( QModelIndex(), idx, idx );
        mEntries.insert( idx, newEntry );
        mOwner->endInsertRows();

        cur = newEntry;
    }

    cur->mPath = path;

    return true;
}

bool FlatTreeModelPrivate::rawRemove( FlatTreeModelEntry* entry )
{
    Q_ASSERT( entry && entry->mFirstChild == NULL );

    if( entry == &mRoot )
        return false;

    FlatTreeModelEntry* parent = entry->mParent;
    FlatTreeModelEntry** ppcur = &parent->mFirstChild;
    while( *ppcur )
    {
        if( *ppcur == entry )
        {
            *ppcur = entry->mNextSibling;

            mOwner->beginRemoveRows( QModelIndex(), entry->mIndex, entry->mIndex );
            mEntries.removeAt( entry->mIndex );
            mOwner->endRemoveRows();

            int curIdx = -1;
            updateIndicies( &mRoot, curIdx, 0, NULL );

            delete entry;

            if( parent->mFirstChild == NULL && parent != &mRoot )
            {
                return rawRemove( parent );
            }
            return true;
        }
    }

    return false;
}

bool FlatTreeModelPrivate::removeEntry( const QString& entry )
{
    QStringList remainder = entry.split( mSeparator );
    FlatTreeModelEntry* cur = subEntry( &mRoot, remainder );

    if( remainder.count() != 0 )
    {
        return false;
    }

    return rawRemove( cur );
}

FlatTreeModelPrivate::~FlatTreeModelPrivate()
{
    qDeleteAll( mEntries );
}

FlatTreeModel::FlatTreeModel( QObject* parent )
    : QAbstractListModel( parent )
{
    d = new FlatTreeModelPrivate( this );
}

FlatTreeModel::FlatTreeModel( const QStringList& entries, QObject* parent )
    : QAbstractListModel( parent )
{
    d = new FlatTreeModelPrivate( this );
    add( entries );
}

FlatTreeModel::FlatTreeModel( const QChar separator, const QStringList& entries, QObject* parent )
    : QAbstractListModel( parent )
{
    d = new FlatTreeModelPrivate( this );
    d->mSeparator = separator;
    add( entries );
}

FlatTreeModel::~FlatTreeModel()
{
    delete d;
}

void FlatTreeModel::clear()
{
    beginResetModel();

    qDeleteAll( d->mEntries );
    d->mRoot.mFirstChild = d->mRoot.mNextSibling = NULL;
    d->mEntries.clear();

    endResetModel();
}

void FlatTreeModel::add( const QStringList& entries )
{
    foreach( QString s, entries )
    {
        add( s );
    }
}

void FlatTreeModel::remove( const QStringList& entries )
{
    foreach( QString s, entries )
    {
        remove( s );
    }
}

void FlatTreeModel::add( const QString& entry )
{
    d->addEntry( entry );
}

void FlatTreeModel::remove( const QString& entry )
{
    d->removeEntry( entry );
}

int FlatTreeModel::rowCount( const QModelIndex& parent ) const
{
    return parent.isValid() ? 0 : d->mEntries.count();
}

QVariant FlatTreeModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() || index.column() != 0 || index.row() >= d->mEntries.count() )
    {
        return QVariant();
    }

    FlatTreeModelEntry* entry = d->mEntries.at( index.row() );

    switch( role )
    {
    case Qt::DisplayRole:
        return entry->mPath;

    case Qt::DecorationRole:
        if( entry->mIcon.isNull() )
        {
            return entry->mIsHeader ? d->mDefaultHeaderIcon
                                    : d->mDefaultEntryIcon;
        }
        return entry->mIcon;

    case Qt::UserRole:
        return entry->mUserData;

    case Qt::UserRole + 1:
        return entry->mIsHeader;

    case Qt::UserRole + 2:
        return entry->mLevel;

    case Qt::UserRole + 3:
        return entry->mText;

    default:
        return QVariant();
    }
}

bool FlatTreeModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if( !index.isValid() || index.column() != 0 || index.row() >= d->mEntries.count() )
    {
        return false;
    }

    FlatTreeModelEntry* entry = d->mEntries.at( index.row() );
    switch( role )
    {
    case Qt::DisplayRole:
        qDebug( "Cannot change a FlatTreeModelEntry's path" );
        return false;

    case Qt::DecorationRole:
        entry->mIcon = value.value< QIcon >();
        emit dataChanged( index, index );
        return true;

    case Qt::UserRole:
        entry->mUserData = value;
        return true;

    default:
        qDebug( "Wrong role given to FlatTreeModel::setData" );
        return false;
    }
}

Qt::ItemFlags FlatTreeModel::flags( const QModelIndex& index ) const
{
    if( !index.isValid() || index.column() != 0 || index.row() >= d->mEntries.count() )
    {
        return Qt::NoItemFlags;
    }

    FlatTreeModelEntry* entry = d->mEntries.at( index.row() );

    if( entry->mIsHeader )
        return Qt::ItemIsEnabled;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QIcon FlatTreeModel::defaultHeaderIcon() const
{
    return d->mDefaultHeaderIcon;
}

QIcon FlatTreeModel::defaultDataIcon() const
{
    return d->mDefaultEntryIcon;
}

void FlatTreeModel::setDefaultHeaderIcon( const QIcon& icon )
{
    beginResetModel();
    d->mDefaultHeaderIcon = icon;
    endResetModel();
}

void FlatTreeModel::setDefaultDataIcon( const QIcon& icon )
{
    beginResetModel();
    d->mDefaultEntryIcon = icon;
    endResetModel();
}
