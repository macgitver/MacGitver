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
#include "libMacGitverCore/RepoMan/RepoMan.hpp"

#include "RepoInfoModel.hpp"

RepoInfoModel::RepoInfoModel()
{
    mRepoMan = &MacGitver::repoMan();

    connect( mRepoMan, SIGNAL(repositoryDeactivated(RepositoryInfo*)),
             this, SLOT(invalidateRepository(RepositoryInfo*)) );

    connect( mRepoMan, SIGNAL(repositoryActivated(RepositoryInfo*)),
             this, SLOT(invalidateRepository(RepositoryInfo*)) );

    connect( mRepoMan, SIGNAL(repositoryOpened(RepositoryInfo*)),
             this, SLOT(repositoryOpened(RepositoryInfo*)));
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
        switch(role) {
        case Qt::DisplayRole:
            return info->displayAlias();

        case Qt::FontRole:
            {
                QFont f1, f2;
                f2.setBold( true );
                return info->isActive() ? f2 : f1;
            }

        case IsActive:
            return info->isActive();

        case Qt::ToolTipRole:
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

void RepoInfoModel::invalidateRepository( RepositoryInfo *info )
{
    if ( !info ) return;

    QModelIndex index = info2Index( info );
    emit dataChanged( index, index );
}

void RepoInfoModel::repositoryOpened(RepositoryInfo *info)
{
    if (!info || info->parentRepository()) {
        return;
    }

    connect(info, SIGNAL(childAdded(RepositoryInfo*,RepositoryInfo*)),
            this, SLOT(repositoryChildAdded(RepositoryInfo*,RepositoryInfo*)));

    // we add a row just at the end of the root. This is stupid. But that's the way it works when
    // a model actually isn't a model...

    int row = mRepoMan->repositories().count() - 1;
    emit beginInsertRows(QModelIndex(), row, row);
    emit endInsertRows();
}

void RepoInfoModel::repositoryChildAdded(RepositoryInfo* parent, RepositoryInfo* child)
{
    QModelIndex parentIndex = info2Index(parent);

    // we add a row just at the end of the root. This is stupid. But that's the way it works when
    // a model actually isn't a model...

    int row = parent->children().count() - 1;
    emit beginInsertRows(parentIndex, row, row);
    emit endInsertRows();
}
