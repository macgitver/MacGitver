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

#include <QFileIconProvider>
#include <QFileInfo>

#include "libGitWrap/Result.hpp"

#include "WorkingTreeModel.h"
#include "WorkingTreeDirItem.h"
#include "WorkingTreeFileItem.h"

#ifdef Q_OS_WIN

#include <windows.h>

QIcon getWindowsIcon( const QString& pathName )
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    SHFILEINFOW shfi;
    memset( &shfi, 0, sizeof(shfi) );
    DWORD flags = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON;
    SHGetFileInfoW( (LPCWSTR) pathName.utf16(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), flags );

    QPixmap pm = QPixmap::fromWinHICON( shfi.hIcon );	// Undefined für Qt5!
    DestroyIcon( shfi.hIcon );
    return QIcon( pm );
#else
    return QIcon();
#endif
}
#endif

WorkingTreeModel::WorkingTreeModel(QObject* parent )
    : QAbstractItemModel( parent )
    , mRootItem( NULL )
{
}

WorkingTreeModel::~WorkingTreeModel()
{
    delete mRootItem;
}

QVariant WorkingTreeModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() )
    {
        return QVariant();
    }

    WorkingTreeAbstractItem* item =
            static_cast< WorkingTreeAbstractItem* >( index.internalPointer() );

    return item->data( index.column(), role );
}

Qt::ItemFlags WorkingTreeModel::flags( const QModelIndex& index ) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant WorkingTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( orientation == Qt::Vertical || role != Qt::DisplayRole )
    {
        return QVariant();
    }

    switch( section )
    {
    case 0:		return trUtf8( "Name" );
    case 1:		return trUtf8( "Size" );
    case 2:		return trUtf8( "M-Time" );
    case 3:		return trUtf8( "Owner" );
    default:	return QVariant();
    }
}

QModelIndex WorkingTreeModel::index( int row, int column, const QModelIndex& parent ) const
{
    if( !hasIndex( row, column, parent ) )
    {
        return QModelIndex();
    }

    WorkingTreeAbstractItem* parentItem;

    if( !parent.isValid() )
        parentItem = mRootItem;
    else
        parentItem = static_cast<WorkingTreeAbstractItem*>( parent.internalPointer() );

    WorkingTreeAbstractItem* childItem = parentItem->childAt( row );
    if( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex WorkingTreeModel::parent( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return QModelIndex();

    WorkingTreeAbstractItem* childItem =
            static_cast< WorkingTreeAbstractItem* >( index.internalPointer() );
    WorkingTreeAbstractItem* parentItem = childItem->parent();

    if( parentItem == mRootItem || parentItem == 0 )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int WorkingTreeModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.column() > 0 )
    {
        return 0;
    }

    WorkingTreeAbstractItem* parentItem = 0;
    if( !parent.isValid() )
        parentItem = mRootItem;
    else
        parentItem = static_cast< WorkingTreeAbstractItem* >( parent.internalPointer() );

    if (parentItem != 0)
        return parentItem->childCount();

    return 0;
}

int WorkingTreeModel::columnCount( const QModelIndex& parent ) const
{
    return 4;
}

void WorkingTreeModel::setRepository( Git::Repository repo )
{
    beginResetModel();

    delete mRootItem;
    mRootItem = new WorkingTreeDirItem( this, NULL );

    mRepo = repo;

    update();

    endResetModel();
}

WorkingTreeAbstractItem *WorkingTreeModel::indexToItem(const QModelIndex &index) const
{
    return static_cast< WorkingTreeAbstractItem * >( index.internalPointer() );
}

void WorkingTreeModel::update()
{
    if( !mRepo.isValid() || (mRootItem == NULL) )
    {
        return;
    }

    QFileIconProvider ip;
    QIcon folderIcon = ip.icon( QFileIconProvider::Folder );

    Git::Result r;
    Git::StatusHash sh = mRepo.status(r);
    Git::StatusHash::ConstIterator it = sh.constBegin();
    while( it != sh.constEnd() )
    {
        Git::StatusFlags curState = Git::StatusFlags( it.value() );

        WorkingTreeDirItem* cur = mRootItem;
        QStringList slNames = it.key().split( L'/' );
        while( slNames.count() > 1 )
        {
            QString dir = slNames.takeFirst();
            WorkingTreeDirItem* next = (WorkingTreeDirItem*) cur->childByName( dir );
            if( !next )
            {
                next = new WorkingTreeDirItem( this, cur );
                next->setName( dir );
                next->setIcon( folderIcon );
                cur->appendItem( next );
            }
            Q_ASSERT( next->isDirectory() );
            cur = (WorkingTreeDirItem*) next;
        }

        Q_ASSERT( slNames.count() == 1 );
        WorkingTreeAbstractItem* fileBase = cur->childByName( slNames[ 0 ] );
        WorkingTreeFileItem* file;
        if( fileBase && fileBase->isDirectory() )
        {
            cur->removeChild( fileBase );
            fileBase = NULL;
        }

        if( !fileBase )
        {
            file = new WorkingTreeFileItem( this, cur );
            file->setName( slNames[ 0 ] );
            cur->appendItem( file );
            fileBase = file;
        }
        else
        {
            Q_ASSERT( !fileBase->isDirectory() );
            file = (WorkingTreeFileItem*) fileBase;
        }

        QFileInfo fi( mRepo.basePath() + L'/' + it.key() );

#ifdef Q_OS_WIN
        file->setIcon( getWindowsIcon( fi.absoluteFilePath() ) );
#else
        file->setIcon( ip.icon( fi ) );
#endif
        file->setSize( fi.size() );
        file->setLastModified( fi.lastModified() );
        file->setOwner( fi.owner() );

        file->setState( curState );

        ++it;
    }

    // TODO: Remove all not visited items (including empty dirs)
}
