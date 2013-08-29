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

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/MacGitver/RepoManager.hpp"

#include "RepoInfoModel.hpp"

RepoInfoModel::RepoInfoModel()
{
    mRepoMan = &MacGitver::repoMan();
}

int RepoInfoModel::rowCount( const QModelIndex& parent ) const
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

int RepoInfoModel::columnCount( const QModelIndex& parent ) const
{
    return 1;
}

QVariant RepoInfoModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() ) return QVariant();

    RepositoryInfo* info = index2Info( index );
    if( info )
    {
        if( role == Qt::DisplayRole )
        {
            return info->displayAlias();
        }
        else if( role == IsActive )
        {
            return info->isActive();
        }
        else if( role == Qt::FontRole )
        {
            QFont f1, f2;
            f2.setBold( true );
            return info->isActive() ? f2 : f1;
        }
        else if ( role == Qt::ToolTipRole )
        {
            return trUtf8( "Branch: %1" ).arg( info->branchDisplay() );
        }
    }

    return QVariant();
}

QModelIndex RepoInfoModel::index( int row, int column, const QModelIndex& parent ) const
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

QModelIndex RepoInfoModel::parent( const QModelIndex& child ) const
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

RepositoryInfo* RepoInfoModel::index2Info( const QModelIndex& index ) const
{
    return static_cast< RepositoryInfo* >( index.internalPointer() );
}

QModelIndex RepoInfoModel::info2Index( RepositoryInfo* info ) const
{
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
