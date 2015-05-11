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

#include "Branches/BranchesViewData.hpp"
#include "RefItem.hpp"

#include "libMacGitverCore/RepoMan/Frontend/Branch.hpp"
#include "libMacGitverCore/RepoMan/Frontend/Repo.hpp"

#include "libGitWrap/Reference.hpp"
#include "libGitWrap/Repository.hpp"

#include <QAbstractItemModel>

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
    void onRefCreated(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
    void onRefDestroyed(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
    void onRefMoved(const RM::Frontend::Repo& repo, const RM::Frontend::Reference& ref);
    void onRefTreeNodeAboutToBeDeleted(const RM::Frontend::Repo& repo, const RM::Frontend::RefTreeNode& obj);

private:
    QModelIndex itemToIndex(RefItem* item) const;
    QModelIndex objectToIndex(const RM::Frontend::Base& obj) const;

    RefItem* link(bool notify, RefItem* it);
    //void insertCollection(RM::CollectionNode* coll);
    RefItem* insertObject(bool notify, const RM::Frontend::Base& obj);
    RefItem* createBranchItem(bool notify, RefItem* parent, const RM::Frontend::Branch& obj);
    RefItem* createTagItem(bool notify, RefItem* parent, const RM::Frontend::Tag& obj);
    RefItem* createScopeItem(bool notify, RefItem* parent, const RM::Frontend::RefTreeNode& obj);
    RefItem* createRemoteItem(bool notify, RefItem* parent, const RM::Frontend::Remote& obj);

    void updatedObject(const RM::Frontend::Base& obj);
    void deletedObject(const RM::Frontend::Base& obj);
    void deleteItem(RefItem* it);

private:
    RM::Frontend::Repo  mRepo;
    RefRoot*            mRoot;

    RefHeadline*        mHeaderLocal;
    RefHeadline*        mHeaderRemote;
    RefHeadline*        mHeaderTags;

    QHash<RM::Frontend::Base, RefItem*>  mObjToItems;
    QHash<RefItem*, RM::Frontend::Base>  mItemToObjs;
};

#endif
