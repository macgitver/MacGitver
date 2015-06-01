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

#include "BranchesModel.hpp"

#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/RepoMan/RepoMan.hpp"
#include "libMacGitverCore/RepoMan/Frontend/Reference.hpp"
#include "libMacGitverCore/RepoMan/Frontend/RefTreeNode.hpp"
#include "libMacGitverCore/RepoMan/Frontend/Branch.hpp"
#include "libMacGitverCore/RepoMan/Frontend/Remote.hpp"

#include "libGitWrap/Result.hpp"

#include <QFont>
#include <QDebug>

BranchesModel::BranchesModel( BranchesViewData* parent )
    : QAbstractItemModel( parent )
    , mRepo(NULL)
    , mRoot(new RefRoot)
    , mHeaderLocal( NULL )
    , mHeaderRemote( NULL )
    , mHeaderTags( NULL )
{
    RM::RepoMan& rm = RM::RepoMan::instance();

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

QModelIndex BranchesModel::objectToIndex(const RM::Frontend::Base& obj) const
{
    return itemToIndex(mObjToItems.value(obj, NULL));
}

int BranchesModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    RefItem* parentItem = indexToItem(parent, mRoot);
    return parentItem->children.count();
}

int BranchesModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant BranchesModel::data(const QModelIndex& index, int role) const
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

RefItem* BranchesModel::createBranchItem(bool notify, RefItem* parent, const RM::Frontend::Branch& obj)
{
    if (obj.name() == QStringLiteral("HEAD")) {
        return nullptr;
    }

    if (!parent /* && obj.parent().objType() != RM::ObjTypes::CollectionNode */) { // ###REPOMAN
        parent = insertObject(notify, obj.parent());
        return nullptr;
    }

    if (!parent) {
        parent = mHeaderLocal;
    }

    return link(notify, new RefBranch(parent, obj));
}

RefItem* BranchesModel::createTagItem(bool notify, RefItem* parent, const RM::Frontend::Tag& obj)
{
    if (!parent /* && obj->parentObject()->objType() != RM::ObjTypes::CollectionNode */) { // ###REPOMAN
        parent = insertObject(notify, obj.parent());
        return NULL;
    }

    if (!parent) {
        parent = mHeaderTags;
    }

    return link(notify, new RefTag(parent, obj));
}

RefItem* BranchesModel::createScopeItem(bool notify, RefItem* parent, const RM::Frontend::RefTreeNode& obj)
{
    if (!parent) {
        RM::Frontend::Base parObj = obj.parent();


        #if 0 // ###REPOMAN
        if (parObj->objType() != RM::ObjTypes::CollectionNode) {
            parent = insertObject(notify, parObj);
            return nullptr;
        }

        RM::Frontend::CollectionNode* cn = static_cast<RM::Frontend::CollectionNode*>(parObj);
        switch (cn->collectionType()) {
        case RM::Frontend::ctTags:
            parent = mHeaderTags;
            break;

        case RM::Frontend::ctBranches:
            parent = mHeaderLocal;
            break;

        default:
            // Should we assert here?
            return NULL;
        }
        #endif
    }

    return link(notify, new RefScope(parent, obj));
}

RefItem* BranchesModel::createRemoteItem(bool notify, RefItem* parent, const RM::Frontend::Remote& obj)
{
    if (!parent) {
        parent = mHeaderRemote;
    }

    return link(notify, new RefRemote(parent, obj));
}

RefItem* BranchesModel::insertObject(bool notify, const RM::Frontend::Base& obj)
{
    if (!obj) {
        return NULL;
    }

    qDebug() << obj.displayName();

    bool     doChildren = false;
    RefItem* parent = mObjToItems.value(obj.parent(), NULL);
    RefItem* it     = mObjToItems.value(obj, NULL);

    if (!it) {

        switch (obj.objType()) {
        case RM::ObjTypes::Branch:
            it = createBranchItem(notify, parent, obj.as<RM::Frontend::Branch>());
            break;

        case RM::ObjTypes::Tag:
            it = createTagItem(notify, parent, obj.as<RM::Frontend::Tag>());
            break;

        case RM::ObjTypes::RefTreeNode:
            it = createScopeItem(notify, parent, obj.as<RM::Frontend::RefTreeNode>());
            doChildren = true;
            break;

        case RM::ObjTypes::Remote:
            it = createRemoteItem(notify, parent, obj.as<RM::Frontend::Remote>());
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
            foreach(RM::Frontend::Base child, obj.children()) {
                insertObject(notify, child);
            }
        }
    }

    return it;
}

void BranchesModel::insertCollection(RM::Frontend::CollectionNode* coll)
{
    foreach(RM::Frontend::Base* obj, coll->childObjects()) {
        insertObject(false, obj);
    }
}

void BranchesModel::updatedObject(RM::Frontend::Base* obj)
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

    RM::Frontend::Base* obj = it->object();
    if (obj) {
        mObjToItems.remove(obj);
        mItemToObjs.remove(it);
    }

    delete it;

    endRemoveRows();
}

void BranchesModel::deletedObject(RM::Frontend::Base* obj)
{
    deleteItem(mObjToItems.value(obj, NULL));
}

void BranchesModel::setRepository(RM::Frontend::Repo* repo)
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

    foreach (RM::Frontend::Remote* rmRemote, repo->childObjects<RM::Frontend::Remote>()) {
        insertObject(false, rmRemote);
    }

    endResetModel();
}

void BranchesModel::onRefCreated(RM::Frontend::Repo* repo, RM::Frontend::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    insertObject(true, ref);
}

void BranchesModel::onRefDestroyed(RM::Frontend::Repo* repo, RM::Frontend::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    deletedObject(ref);
}

void BranchesModel::onRefTreeNodeAboutToBeDeleted(RM::Frontend::Repo* repo, RM::Frontend::RefTreeNode* obj)
{
    if (repo != mRepo) {
        return;
    }

    deletedObject(obj);
}

void BranchesModel::onRefMoved(RM::Frontend::Repo* repo, RM::Frontend::Ref* ref)
{
    if (repo != mRepo) {
        return;
    }

    updatedObject(ref);
}
