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

#ifndef MGV_BRANCHES_MODEL_HPP
#define MGV_BRANCHES_MODEL_HPP

#include <QAbstractItemModel>

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Repository.hpp"

#include "Branches/BranchesViewData.hpp"
#include "RefItem.hpp"

namespace RM
{
    class Ref;
    class Repo;
    class CollectionNode;
}

class RefItem;
class RefScope;

class BranchesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BranchesModel(BranchesViewData* parent);
    ~BranchesModel();

public:
    void setRepository(RM::Repo* repo);
    RefItem* indexToItem(const QModelIndex& index, RefItem* defaultItem = NULL) const;

public:
    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;
    QModelIndex index( int row, int column = 0, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    bool hasChildren( const QModelIndex& parent ) const;

signals:
    void gitError( const Git::Result& error );

private slots:
    void onRefCreated(RM::Repo* repo, RM::Ref* ref);
    void onRefDestroyed(RM::Repo* repo, RM::Ref* ref);
    void onRefMoved(RM::Repo* repo, RM::Ref* ref);
    void onRefTreeNodeAboutToBeDeleted(RM::Repo* repo, RM::RefTreeNode* obj);

private:
    QModelIndex itemToIndex(RefItem* item) const;
    QModelIndex objectToIndex(RM::Base* obj) const;

    RefItem* link(bool notify, RefItem* it);
    void insertCollection(RM::CollectionNode* coll);
    RefItem* insertObject(bool notify, RM::Base* obj);
    RefItem* createBranchItem(bool notify, RefItem* parent, RM::Branch* obj);
    RefItem* createTagItem(bool notify, RefItem* parent, RM::Tag* obj);
    RefItem* createScopeItem(bool notify, RefItem* parent, RM::RefTreeNode* obj);
    RefItem* createRemoteItem(bool notify, RefItem* parent, RM::Remote* obj);

    void updatedObject(RM::Base* obj);
    void deletedObject(RM::Base* obj);
    void deleteItem(RefItem* it);

private:
    RM::Repo*           mRepo;
    RefRoot*            mRoot;

    RefHeadline*        mHeaderLocal;
    RefHeadline*        mHeaderRemote;
    RefHeadline*        mHeaderTags;

    QHash<RM::Base*, RefItem*>  mObjToItems;
    QHash<RefItem*, RM::Base*>  mItemToObjs;
};

#endif
