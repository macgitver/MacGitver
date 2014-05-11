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

#include "libHeavenIcons/IconRef.hpp"
#include "libHeavenIcons/Icon.hpp"

#include "RepoInfoModel.hpp"

RepoInfoModel::RepoInfoModel()
{
    mRepoMan = &MacGitver::repoMan();

    connect( mRepoMan, SIGNAL(repositoryDeactivated(RM::Repo*)),
             this,     SLOT(invalidateRepository(RM::Repo*)) );

    connect( mRepoMan, SIGNAL(repositoryActivated(RM::Repo*)),
             this,     SLOT(invalidateRepository(RM::Repo*)) );

    connect( mRepoMan, SIGNAL(repositoryOpened(RM::Repo*)),
             this,     SLOT(repositoryOpened(RM::Repo*)));
}

int RepoInfoModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.isValid() )
    {
        RM::Repo* info = index2Info( parent );
        return info ? info->submodules().count() : 0;
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

    RM::Repo* info = index2Info( index );
    if( info )
    {
        switch(role) {
        case Qt::DisplayRole:
            return info->displayAlias();

        case Qt::DecorationRole:
            return info->icon().icon().pixmap();

        case Qt::FontRole:
            {
                QFont f1, f2;
                f2.setBold( true );
                return info->isActive() ? f2 : f1;
            }

        case IsActive:
            return info->isActive();

        case Qt::ToolTipRole:
            return QLatin1String("<html><head><style type=\"text/css\">"
                                 ".h1 { margin: 0px; font-size: large; color: gray; }"
                                 ".row { margin: 0px; }"
                                 "</style></head><body>" ) +
                    trUtf8( "<div class=\"h1\">%1</div>" ).arg( info->displayName() ) +
                    QLatin1String("<hr/>") +
                    trUtf8( "<div class=\"row\">Branch: %1</div>" ).arg( info->branchDisplay() ) +
                    trUtf8( "</body></html>" )
                    ;
        }
    }

    return QVariant();
}

QModelIndex RepoInfoModel::index( int row, int column, const QModelIndex& parent ) const
{
    Q_UNUSED(column);
    RM::Repo::List list;

    if( parent.isValid() )
    {
        RM::Repo* infoParent = index2Info( parent );
        if( !infoParent )
        {
            return QModelIndex();
        }

        foreach (RM::Repo* r, infoParent->submodules()) {
            list.append(r);
        }
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

    RM::Repo* info = index2Info( child );
    if( !info || !info->parentRepository() )
    {
        return QModelIndex();
    }

    return info2Index( info->parentRepository() );
}

RM::Repo* RepoInfoModel::index2Info( const QModelIndex& index ) const
{
    return static_cast< RM::Repo* >( index.internalPointer() );
}

QModelIndex RepoInfoModel::info2Index(RM::Repo* info) const
{
    int row = 0;

    if( !info )
    {
        return QModelIndex();
    }

    if( info->parentRepository() )
    {
        RM::Repo::Set sms = info->parentRepository()->submodules();
        RM::Repo::List list;
        foreach (RM::Repo* r, sms) {
            list.append(r);
        }
        row = list.indexOf(info);
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

void RepoInfoModel::invalidateRepository(RM::Repo *info)
{
    if ( !info ) return;

    QModelIndex index = info2Index( info );
    emit dataChanged( index, index );
}

void RepoInfoModel::repositoryOpened(RM::Repo *info)
{
    if (!info || info->parentRepository()) {
        return;
    }

    connect(info, SIGNAL(childAdded(RM::Repo*,RM::Repo*)),
            this, SLOT(repositoryChildAdded(RM::Repo*,RM::Repo*)));

    // we add a row just at the end of the root. This is stupid. But that's the way it works when
    // a model actually isn't a model...

    int row = mRepoMan->repositories().count() - 1;
    emit beginInsertRows(QModelIndex(), row, row);
    emit endInsertRows();
}

void RepoInfoModel::repositoryChildAdded(RM::Repo* parent, RM::Repo* child)
{
    QModelIndex parentIndex = info2Index(parent);

    // we add a row just at the end of the root. This is stupid. But that's the way it works when
    // a model actually isn't a model...

    int row = parent->submodules().count() - 1;
    emit beginInsertRows(parentIndex, row, row);
    emit endInsertRows();
}
