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

#include <QStringList>
#include <QSet>

#include "RefsListModel.h"
#include "RefItem.hpp"

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Repository.hpp"
#include "libGitWrap/Result.hpp"


RefsListModel::RefsListModel( QObject* parent )
{
}

RefsListModel::~RefsListModel()
{
}

int RefsListModel::rowCount( const QModelIndex& parent ) const
{
    return mOrderedData.count();
}

QVariant RefsListModel::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() || index.parent().isValid() )
    {
        return QVariant();
    }

    switch( role )
    {
    case Qt::DisplayRole:
        return mOrderedData.at( index.row() );
    }

    return QVariant();
}

void RefsListModel::updateRef( const QString& refName )
{
}

void RefsListModel::addRef( const Git::Reference& ref )
{
    if ( ref.isValid() )
        return;

    int row = mOrderedData.count();

    beginInsertRows( QModelIndex(), row, row );
    mData.insert( ref.name(), new RefBranch( 0, ref.shorthand(), ref ) );

    mOrderedData.insert( row, ref.name() );
    endInsertRows();
}

void RefsListModel::removeRef( const QString& refName )
{
}

void RefsListModel::sync(const Git::Repository &repo )
{
//    if ( mRepo != repo )
        mRepo = repo;

    if( !mRepo.isValid() )
    {
        clear();
        return;
    }

    QSet< QString > oldRefs = mData.keys().toSet();
    Git::Result r;
    Git::ReferenceList refs = mRepo.allReferences( r );
    if ( !r ) return;

    foreach( const Git::Reference &ref, refs )
    {
        if( oldRefs.contains( ref.name() ) )
        {
            oldRefs.remove( ref.name() );
        }
        if( mData.contains( ref.shorthand() ) )
        {
            updateRef( ref.shorthand() );
        }
        else
        {
            addRef( ref );
        }
    }

    foreach( QString refName, oldRefs )
    {
        removeRef( refName );
    }
}

void RefsListModel::clear()
{
    beginResetModel();
    mData.clear();
    endResetModel();

    //qDeleteAll( mOrderedData );
    mOrderedData.clear();
}
