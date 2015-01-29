/*
 * MacGitver
 * Copyright (C) 2015 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Sascha Cunz <sascha@macgitver.org>
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

#include <QFont>

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Ref.hpp"

#include "libGitWrap/Result.hpp"
#include "libGitWrap/Reference.hpp"

#include "BranchesModel.hpp"

#include "RefItem.hpp"


BranchesModel::BranchesModel( BranchesViewData* parent )
    : QAbstractItemModel( parent )
    , mData( parent )
    , mRoot( new RefItem )
{
    RM::RepoMan& rm = MacGitver::repoMan();
    connect( &rm, SIGNAL(refCreated(RM::Repo*,RM::Ref*)), this, SLOT(onRefCreated(RM::Repo*,RM::Ref*)) );
}

BranchesModel::~BranchesModel()
{
    delete mRoot;
}

int BranchesModel::rowCount( const QModelIndex& parent ) const
{
    RefItem* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    return parentItem->children.count();
}

int BranchesModel::columnCount( const QModelIndex& parent ) const
{
    return 1;
}

QVariant BranchesModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() )
    {
        return QVariant();
    }

    RefItem* item = static_cast< RefItem* >( index.internalPointer() );
    return item->data( index.column(), role );
}

bool BranchesModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( !index.isValid() || (role != Qt::EditRole) )
        return false;

    RefItem *item = static_cast<RefItem *>( index.internalPointer() );
    if ( !item )
        return false;

    Git::Result result;
    if ( !item->setData( result, value, role, index.column() ) )
    {
        if( !result )
            emit gitError( result );

        return false;
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags BranchesModel::flags( const QModelIndex& index ) const
{
    if ( !index.isValid() )
        return Qt::NoItemFlags;

    Qt::ItemFlags result = Qt::ItemIsEnabled;
    const RefItem *item = static_cast<const RefItem *>( index.internalPointer() );

    RefItem::ItemType t = static_cast<RefItem::ItemType>(
                item->data( 0, RefItem::TypeRole ).toInt() );
    if ( (t == RefItem::Reference) || (t == RefItem::Namespace) )
        result |= Qt::ItemIsSelectable;

    if ( item->isEditable() )
        result |= Qt::ItemIsEditable;

    return result;
}

QModelIndex BranchesModel::index( int row, int column, const QModelIndex& parent ) const
{
    if( !hasIndex( row, column, parent ) )
    {
        return QModelIndex();
    }

    RefItem* parentItem;

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    RefItem* childItem = parentItem->children.at( row );
    if( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex BranchesModel::parent( const QModelIndex& child ) const
{
    if( !child.isValid() )
    {
        return QModelIndex();
    }

    RefItem* childItem = static_cast< RefItem* >( child.internalPointer() );
    RefItem* parentItem = childItem->parent;

    if( parentItem == mRoot )
        return QModelIndex();

    int row = parentItem->parent->children.indexOf( parentItem );
    return createIndex( row, 0, parentItem );
}

bool BranchesModel::hasChildren( const QModelIndex& parent ) const
{
    RefItem* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< RefItem* >( parent.internalPointer() );

    return parentItem->children.count() > 0;
}

void BranchesModel::insertRef(bool notify, const Git::Reference &ref)
{
    RefScope* scope = scopeForRef( ref );
    Q_ASSERT( scope );

    QStringList parts = ref.shorthand().split( QChar( L'/' ) );
    if ( parts.count() == 1 )
    {
        insertBranch( notify, scope, parts.last(), ref );
        return;
    }

    RefItem* ns = scope;
    for( int j = 0; j < parts.count() - 1; j++ )
    {
        RefItem* next = NULL;
        QString partName = parts[ j ];
        foreach( RefItem* nsChild, ns->children )
        {
            if( nsChild->text() == partName )
            {
                next = nsChild;
                break;
            }
        }

        if( !next )
        {
            next = insertNamespace( notify, ns, partName );
        }
        ns = next;
    }

    Q_ASSERT( ns );

    insertBranch( notify, ns, parts.last(), ref );
}

RefItem* BranchesModel::insertNamespace(const bool notify, RefItem* parent, const QString& name)
{
    RefItem* next = NULL;
    if ( notify ) {
        int fr = parent->children.count();
        beginInsertRows( index( parent ), fr, fr );
    }

    next = new RefNameSpace( parent, name );

    if ( notify ) {
        endInsertRows();
    }
    return next;
}

void BranchesModel::insertBranch(const bool notify, RefItem* parent, const QString& name, const Git::Reference& ref)
{
    if ( notify ) {
        int row = parent->children.count();
        beginInsertRows( index( parent ), row, row );
    }

    new RefBranch( parent, ref );

    if (notify) {
        endInsertRows();
    }
}

RefScope* BranchesModel::scopeForRef(const Git::Reference& ref) const
{
    RefItem* scope = NULL;
    if ( ref.isLocal() )        scope = mRoot->children[0];
    else if ( ref.isRemote() )  scope = mRoot->children[1];
    else scope = mRoot->children[2];

    return static_cast< RefScope* >( scope );
}

void BranchesModel::rereadBranches()
{
    beginResetModel();

    qDeleteAll( mRoot->children );
    mRoot->children.clear();

    new RefScope( mRoot, tr( "Local" ) );
    new RefScope( mRoot, tr( "Remote" ) );
    new RefScope( mRoot, tr( "Tags" ) );

    RM::Repo* repo = mData->repository();
    Q_ASSERT( repo );

    // TODO: replace git repo with RM::Repo
    Git::Repository gitRepo = repo ? repo->gitRepo() : Git::Repository();

    if( gitRepo.isValid() )
    {
        Git::Result r;
        Git::ReferenceList sl = repo.allReferences( r );
        if( !sl.isEmpty() )
        {
            for( int i = 0; i < sl.count(); ++i )
            {
                const Git::Reference &currentRef = sl[ i ];
                insertRef( false, currentRef );
            }
        }
    }

    endResetModel();
}

void BranchesModel::onRefCreated(RM::Repo* repo, RM::Ref* ref)
{
    Git::Result r;
    Git::Reference gref = repo->gitRepo().reference( r, ref->fullName() );
    Q_ASSERT( r );

    insertRef( true, gref );
}

QModelIndex BranchesModel::index(RefItem* item) const
{
    if ( !item || (item == mRoot) )
    {
        return QModelIndex();
    }

    RefItem* parent = item->parent ? item->parent : mRoot;
    int row = parent->children.indexOf( item );
    return createIndex( row, 0, item );
}
