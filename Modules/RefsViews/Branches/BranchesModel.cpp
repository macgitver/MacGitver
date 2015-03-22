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
#include <QDebug>

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Ref.hpp"
#include "libMacGitverCore/RepoMan/RefTreeNode.hpp"
#include "libMacGitverCore/RepoMan/Branch.hpp"
#include "libMacGitverCore/RepoMan/Remote.hpp"
#include "libMacGitverCore/RepoMan/CollectionNode.hpp"

#include "libGitWrap/Result.hpp"

#include "BranchesModel.hpp"


BranchesModel::BranchesModel( BranchesViewData* parent )
    : QAbstractItemModel( parent )
    , mRepo(NULL)
    , mRoot(new RefRoot)
    , mHeaderLocal( NULL )
    , mHeaderRemote( NULL )
    , mHeaderTags( NULL )
{
    RM::RepoMan& rm = MacGitver::repoMan();

    connect(&rm,  &RM::RepoMan::refCreated,
            this, &BranchesModel::onRefCreated);

    connect(&rm,  &RM::RepoMan::refAboutToBeDeleted,
            this, &BranchesModel::onRefDestroyed);

    connect(&rm,  &RM::RepoMan::refMoved,
            this, &BranchesModel::onRefMoved);

    connect(&rm,  &RM::RepoMan::refTreeNodeAboutToBeDeleted,
            this, &BranchesModel::onRefTreeNodeAboutToBeDeleted);
}

BranchesModel::~BranchesModel()
{
    delete mRoot;
}

RefItem* BranchesModel::indexToItem(const QModelIndex& index, RefItem* defaultItem) const
{
    return index.isValid() ? static_cast< RefItem* >( index.internalPointer() ) : defaultItem;
}

QModelIndex BranchesModel::itemToIndex(RefItem* item) const
{
    if (!item || (item == mRoot)) {
        return QModelIndex();
    }

    RefItem* parent = item->parent ? item->parent : mRoot;
    int row = parent->children.indexOf( item );
    return createIndex( row, 0, item );
}

QModelIndex BranchesModel::objectToIndex(RM::Base* obj) const
{
    return itemToIndex(mObjToItems.value(obj, NULL));
}

int BranchesModel::rowCount( const QModelIndex& parent ) const
{
    if( parent.column() > 0 )
    {
        return 0;
    }

    RefItem* parentItem = indexToItem(parent, mRoot);
    return parentItem->children.count();
}

int BranchesModel::columnCount( const QModelIndex& parent ) const
{
    return 1;
}

QVariant BranchesModel::data( const QModelIndex& index, int role ) const
{
    RefItem* item = indexToItem(index);
    if (index.column() != 0) {
        return QVariant();
    }
    return item ? item->data(role) : QVariant();
}

Qt::ItemFlags BranchesModel::flags( const QModelIndex& index ) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    RefItem* item = indexToItem(index);
    RefItem::ItemType t = item->type();

    if (t == RefItem::Headline) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QModelIndex BranchesModel::index( int row, int column, const QModelIndex& parent ) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    RefItem* parentItem = indexToItem(parent, mRoot);
    RefItem* childItem = parentItem->children.at( row );

    return childItem ? createIndex( row, column, childItem ) : QModelIndex();
}

QModelIndex BranchesModel::parent( const QModelIndex& child ) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    RefItem* childItem = indexToItem(child);
    RefItem* parentItem = childItem->parent;

    if (parentItem == mRoot) {
        return QModelIndex();
    }

    int row = parentItem->parent->children.indexOf( parentItem );
    return createIndex( row, 0, parentItem );
}

bool BranchesModel::hasChildren(const QModelIndex& parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    RefItem* parentItem = indexToItem(parent, mRoot);
    return parentItem->children.count() > 0;
}

RefItem* BranchesModel::link(bool notify, RefItem* it)
{
    int pos = it->itemPosition();

    if (notify) {
        beginInsertRows(itemToIndex(it->parent), pos, pos);
    }

    it->parent->children.insert(pos, it);

    if (notify) {
        endInsertRows();
    }

    return it;
}

RefItem* BranchesModel::createBranchItem(bool notify, RefItem* parent, RM::Branch* obj)
{
    if (obj->name() == QStringLiteral("HEAD")) {
        return NULL;
    }

    if (!parent && obj->parentObject()->objType() != RM::CollectionNodeObject) {
        parent = insertObject(notify, obj->parentObject());
        return NULL;
    }

    if (!parent) {
        parent = mHeaderLocal;
    }

    return link(notify, new RefBranch(parent, obj));
}

RefItem* BranchesModel::createTagItem(bool notify, RefItem* parent, RM::Tag* obj)
{
    if (!parent && obj->parentObject()->objType() != RM::CollectionNodeObject) {
        parent = insertObject(notify, obj->parentObject());
        return NULL;
    }

    if (!parent) {
        parent = mHeaderTags;
    }

    return link(notify, new RefTag(parent, obj));
}

RefItem* BranchesModel::createScopeItem(bool notify, RefItem* parent, RM::RefTreeNode* obj)
{
    if (!parent) {
        RM::Base* parObj = obj->parentObject();

        if (parObj->objType() != RM::CollectionNodeObject) {
            parent = insertObject(notify, parObj);
            return NULL;
        }

        RM::CollectionNode* cn = static_cast<RM::CollectionNode*>(parObj);
        switch (cn->collectionType()) {
        case RM::ctTags:
            parent = mHeaderTags;
            break;

        case RM::ctBranches:
            parent = mHeaderLocal;
            break;

        default:
            // Should we assert here?
            return NULL;
        }
    }

    return link(notify, new RefScope(parent, obj));
}

RefItem* BranchesModel::createRemoteItem(bool notify, RefItem* parent, RM::Remote* obj)
{
    if (!parent) {
        parent = mHeaderRemote;
    }

    return link(notify, new RefRemote(parent, obj));
}

RefItem* BranchesModel::insertObject(bool notify, RM::Base* obj)
{
    if (!obj) {
        return NULL;
    }

    qDebug() << obj->displayName();

    bool     doChildren = false;
    RefItem* parent = mObjToItems.value(obj->parentObject(), NULL);
    RefItem* it     = mObjToItems.value(obj, NULL);

    if (!it) {
        switch (obj->objType()) {
        case RM::BranchObject:
            it = createBranchItem(notify, parent, static_cast<RM::Branch*>(obj));
            break;

        case RM::TagObject:
            it = createTagItem(notify, parent, static_cast<RM::Tag*>(obj));
            break;

        case RM::RefTreeNodeObject:
            it = createScopeItem(notify, parent, static_cast<RM::RefTreeNode*>(obj));
            doChildren = true;
            break;

        case RM::RemoteObject:
            it = createRemoteItem(notify, parent, static_cast<RM::Remote*>(obj));
            doChildren = true;
            break;

        default:
            break;
        }
    }

    if (it) {
        mObjToItems.insert(obj, it);
        mItemToObjs.insert(it, obj);

        if (doChildren) {
            foreach(RM::Base* child, obj->childObjects()) {
                insertObject(notify, child);
            }
        }
    }

    return it;
}

void BranchesModel::insertCollection(RM::CollectionNode* coll)
{
    foreach(RM::Base* obj, coll->childObjects()) {
        insertObject(false, obj);
    }
}

void BranchesModel::updatedObject(RM::Base* obj)
{
    RefItem* it = mObjToItems.value(obj, NULL);
    if (!it) {
        return;
    }

    QModelIndex i = itemToIndex(it);
    dataChanged(i, i);
}

void BranchesModel::deleteItem(RefItem* it)
{
    if (!it) {
        return;
    }

    // Do a depth-first traversal, so we're sure everything's removed correctly

    foreach (RefItem* sub, it->children) {
        deleteItem(sub);
    }

    RefItem* parent = it->parent;
    int pos = parent->children.indexOf(it);

    beginRemoveRows(itemToIndex(parent), pos, pos);

    RM::Base* obj = it->object();
    if (obj) {
        mObjToItems.remove(obj);
        mItemToObjs.remove(it);
    }

    delete it;

    endRemoveRows();
}

void BranchesModel::deletedObject(RM::Base* obj)
{
    deleteItem(mObjToItems.value(obj, NULL));
}

void BranchesModel::setRepository(RM::Repo* repo)
{
    if (mRepo == repo) {
        return;
    }

    beginResetModel();

    if (mRepo) {
        qDeleteAll(mRoot->children);
        mObjToItems.clear();
        mItemToObjs.clear();
        mHeaderLocal = mHeaderRemote = mHeaderTags = NULL;
    }

    mRepo = repo;

    mHeaderLocal    = new RefHeadline(mRoot, tr("Local branches")); link(false, mHeaderLocal);
    mHeaderRemote   = new RefHeadline(mRoot, tr("Remotes"));        link(false, mHeaderRemote);
    mHeaderTags     = new RefHeadline(mRoot, tr("Tags"));           link(false, mHeaderTags);

    insertCollection(mRepo->branches());
    insertCollection(mRepo->tags());

    foreach (RM::Remote* rmRemote, repo->childObjects<RM::Remote>()) {
        insertObject(false, rmRemote);
    }

    endResetModel();
}

void BranchesModel::onRefCreated(RM::Repo* repo, RM::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    insertObject(true, ref);
}

void BranchesModel::onRefDestroyed(RM::Repo* repo, RM::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    deletedObject(ref);
}

void BranchesModel::onRefTreeNodeAboutToBeDeleted(RM::Repo* repo, RM::RefTreeNode* obj)
{
    if (repo != mRepo) {
        return;
    }

    deletedObject(obj);
}

void BranchesModel::onRefMoved(RM::Repo* repo, RM::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    updatedObject(ref);
}
