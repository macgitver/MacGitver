/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
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

#include "libGitWrap/Result.hpp"

#include "BranchesModel.hpp"

class BranchesModel::Item
{
public:
    Item()
        : parent( NULL )
        , text()
    {}

    Item( Item* p, const QString& t )
        : parent( p )
        , text( t )
    {
        Q_ASSERT( p );
        p->children.append( this );
    }

    virtual ~Item()
    {
        if( parent )
        {
            parent->children.removeOne( this );
        }
        qDeleteAll( children );
    }

public:
    Item* parent;
    QList< Item* > children;
    QString text;

    virtual QVariant data( int col, int role ) const { return QVariant(); }
};

class BranchesModel::Scope : public BranchesModel::Item
{
public:
    Scope( Item* p, const QString& t )
        : Item( p, t ) {}

    QVariant data( int col, int role ) const
    {
        switch( role )
        {
        case Qt::DisplayRole:
            return text;
        }

        return QVariant();
    }
};

class BranchesModel::Remote : public BranchesModel::Item
{
public:
    Remote( Item* p, const QString& t )
        : Item( p, t ) {}

    QVariant data( int col, int role ) const
    {
        switch( role )
        {
        case Qt::DisplayRole:
            return text;
        }

        return QVariant();
    }
};

class BranchesModel::NameSpace : public BranchesModel::Item
{
public:
    NameSpace( Item* p, const QString& t )
        : Item( p, t ) {}

    QVariant data( int col, int role ) const
    {
        switch( role )
        {
        case Qt::DisplayRole:
            return text;
        }

        return QVariant();
    }
};

class BranchesModel::Branch : public BranchesModel::Item
{
public:
    Branch( Item* p, const QString& t )
        : Item( p, t ), cur( false ) {}

    QVariant data( int col, int role ) const
    {
        switch( role )
        {
        case Qt::DisplayRole:
            return text;

        case Qt::FontRole:
            if( cur )
            {
                QFont f;
                f.setBold( true );
                return f;
            }
            break;
        }

        return QVariant();
    }

    bool cur;
};

BranchesModel::BranchesModel( BranchesViewData* parent )
    : QAbstractItemModel( parent )
    , mData( parent )
{
    mRoot = new Item;
    mRoot->parent = NULL;
}

BranchesModel::~BranchesModel()
{
    delete mRoot;
}

int BranchesModel::rowCount( const QModelIndex& parent ) const
{
    Item* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< Item* >( parent.internalPointer() );

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

    Item* item = static_cast< Item* >( index.internalPointer() );
    return item->data( index.column(), role );
}

bool BranchesModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    Q_ASSERT( false );
    return false;
}

Qt::ItemFlags BranchesModel::flags( const QModelIndex& index ) const
{
    Q_UNUSED( index );
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QModelIndex BranchesModel::index( int row, int column, const QModelIndex& parent ) const
{
    if( !hasIndex( row, column, parent ) )
    {
        return QModelIndex();
    }

    Item* parentItem;

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< Item* >( parent.internalPointer() );

    Item* childItem = parentItem->children.at( row );
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

    Item* childItem = static_cast< Item* >( child.internalPointer() );
    Item* parentItem = childItem->parent;

    if( parentItem == mRoot )
        return QModelIndex();

    int row = parentItem->parent->children.indexOf( parentItem );
    return createIndex( row, 0, parentItem );
}

bool BranchesModel::hasChildren( const QModelIndex& parent ) const
{
    Item* parentItem;
    if( parent.column() > 0 )
    {
        return 0;
    }

    if( !parent.isValid() )
        parentItem = mRoot;
    else
        parentItem = static_cast< Item* >( parent.internalPointer() );

    return parentItem->children.count() > 0;
}

void BranchesModel::rereadBranches()
{
    Git::Repository repo = mData->repository();

    beginResetModel();

    foreach( Item* i, mRoot->children )
        delete i;
    Q_ASSERT( mRoot->children.count() == 0 );
    mRoot->children.empty();

    if( repo.isValid() )
    {
        Git::Result r;
        QString curBranch = repo.currentBranch( r );

        QStringList sl = repo.branchNames( r, true, false );
        if( sl.count() )
        {
            Scope* scope = new Scope( mRoot, tr( "Local" ) );

            for( int i = 0; i < sl.count(); i++ )
            {
                Branch* branch;
                QString branchName = sl[ i ];
                QStringList parts = branchName.split( QChar( L'/' ) );
                if( parts.count() == 1 )
                {
                    branch = new Branch( scope, branchName );
                }
                else
                {
                    Item* ns = scope;
                    QString totPart;
                    for( int j = 0; j < parts.count() - 1; j++ )
                    {
                        Item* next = NULL;
                        QString partName = parts[ j ];
                        totPart += partName + QChar( L'/' );
                        foreach( Item* nsChild, ns->children )
                        {
                            if( nsChild->text == partName ) // + Type
                            {
                                next = nsChild;
                                break;
                            }
                        }
                        if( !next )
                        {
                            next = new NameSpace( ns, partName );
                        }
                        ns = next;
                    }

                    Q_ASSERT( ns );
                    branch = new Branch( ns, parts.last() );
                }

                if( branchName == curBranch )
                {
                    branch->cur = true;
                }
            }
        }

        sl = repo.branchNames( r, false, true );
        if( sl.count() )
        {
            QHash< QString, Remote* > remotes;

            for( int i = 0; i < sl.count(); i++ )
            {
                QString branchName = sl[ i ];
                QStringList parts = branchName.split( QChar( L'/' ) );

                QString remoteName = parts.takeFirst();
                branchName = parts.join( QString( L'/' ) );

                Remote* scope = remotes.value( remoteName, NULL );
                if( !scope )
                {
                    scope = new Remote( mRoot, remoteName );
                    remotes[ remoteName ] = scope;
                }

                if( parts.count() == 1 )
                {
                    if( branchName != QLatin1String( "HEAD" ) )
                    {
                        new Branch( scope, branchName );
                    }
                }
                else
                {
                    Item* ns = scope;
                    QString totPart;
                    for( int j = 0; j < parts.count() - 1; j++ )
                    {
                        Item* next = NULL;
                        QString partName = parts[ j ];
                        totPart += partName + QChar( L'/' );
                        foreach( Item* nsChild, ns->children )
                        {
                            if( nsChild->text == partName ) // + Type
                            {
                                next = nsChild;
                                break;
                            }
                        }
                        if( !next )
                        {
                            next = new NameSpace( ns, partName );
                        }
                        ns = next;
                    }

                    Q_ASSERT( ns );
                    if( parts.last() != QLatin1String( "HEAD" ) )
                    {
                        new Branch( ns, parts.last() );
                    }
                }
            }
        }
    }

    endResetModel();
}

