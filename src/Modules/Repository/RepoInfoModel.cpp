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

#include "MacGitver/MacGitver.h"
#include "MacGitver/RepoManager.hpp"

#include "RepoInfoModel.hpp"

RepositoryInfoModel::RepositoryInfoModel()
{
    mRepoMan = MacGitver::self().repoMan();
}

int RepositoryInfoModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        RepositoryInfo* info = index2Info( parent );
        return info ? info->children().count() : 0;
    }
    else
    {
        return mRepoMan->repositories().count();
    }
}

int RepositoryInfoModel::columnCount( const QModelIndex& parent ) const
{
    return 1;
}

QVariant RepositoryInfoModel::data( const QModelIndex& index, int role ) const
{
    if( role == Qt::DisplayRole )
    {
        if( index.isValid() )
        {
            RepositoryInfo* info = index2Info( index );
            if( info )
            {
                return info->displayAlias();
            }
        }
    }
    return QVariant();
}

QModelIndex RepositoryInfoModel::index( int row, int column, const QModelIndex& parent ) const
{
    RepositoryInfo::List list;

    if( parent.isValid() )
    {
        RepositoryInfo* infoParent = index2Info( parent );
        if( !infoParent )
        {
            return QModelIndex();
        }

        list = infoParent->children();
    }
    else
    {
        list = mRepoMan->repositories();
    }

    if( row >= list.count() || row < 0 )
    {
        return QModelIndex();
    }

    return info2Index( list.at( row ) );
}

QModelIndex RepositoryInfoModel::parent( const QModelIndex& child ) const
{
    if( !child.isValid() )
    {
        return QModelIndex();
    }

    RepositoryInfo* info = index2Info( child );
    if( !info || !info->parentRepository() )
    {
        return QModelIndex();
    }

    return info2Index( info->parentRepository() );
}

RepositoryInfo* RepositoryInfoModel::index2Info( const QModelIndex& index ) const
{
    return static_cast< RepositoryInfo* >( index.internalPointer() );
}

QModelIndex RepositoryInfoModel::info2Index( RepositoryInfo* info ) const
{
    // QModelIndex parentIndex;
    int row = 0;

    if( !info )
    {
        return QModelIndex();
    }

    if( info->parentRepository() )
    {
        row = info->parentRepository()->children().indexOf( info );
    }
    else
    {
        row = mRepoMan->repositories().indexOf( info );
    }

    if( row == -1 )
    {
        return QModelIndex();
    }

    return createIndex( row, 0, info );
}
