/*
 * MacGitver
 * Copyright (C) 2012-2013 The MacGitver-Developers <dev@macgitver.org>
 *
 * (C) Nils Fenner <nilsfenner@web.de>
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

#pragma once

#include "libGitWrap/Reference.hpp"

#include "libHeavenIcons/Icon.hpp"

#include "libMacGitverCore/RepoMan/RefTreeNode.hpp"
#include "libMacGitverCore/RepoMan/Ref.hpp"
#include "libMacGitverCore/RepoMan/Branch.hpp"
#include "libMacGitverCore/RepoMan/Tag.hpp"
#include "libMacGitverCore/RepoMan/Remote.hpp"

#include <QList>
#include <QVariant>
#include <QPixmap>

namespace RM
{
    class Base;
    class Ref;
    class Branch;
    class RefTreeNode;
    class Namespace;
}

class BranchesModel;

class RefItem
{
public:
    enum Role
    {
        RowBgRole = Qt::UserRole + 1,
        RowBgGradientRole = Qt::UserRole + 2
    };

    enum ItemType
    {
        Root = 1,
        Headline,
        Scope,
        Namespace,
        Remote,
        Reference,
        Branch = Reference,
        Tag
    };

protected:
    RefItem();
    RefItem(RefItem* parent);

public:
    virtual ~RefItem();

public:
    int itemPosition();
    virtual ItemType type() const = 0;
    virtual RM::Base* object();

public:
    RefItem* parent;
    QList< RefItem* > children;

    virtual QVariant data(int role) const;
};

class RefRoot : public RefItem
{
public:
    enum { StaticType = Root };
public:
    RefRoot() {}

public:
    ItemType type() const;
};

class RefHeadline : public RefItem
{
public:
    enum { StaticType = Headline };

public:
    RefHeadline(RefItem* parent, const QString& t)
        : RefItem(parent)
        , mText(t)
    {}

public:
    int itemPosition();
    virtual QVariant data(int role) const;
    ItemType type() const;

public:
    QString mText;
};

template<class T>
class RefItemObject : public RefItem
{
protected:
    RefItemObject(RefItem* parent, T* object)
        : RefItem(parent)
        , mObject(object)
    {}

public:
    const T* object() const {
        return mObject;
    }

    T* object() {
        return mObject;
    }

    QVariant data(int role) const {
        if (mObject) {
            switch (role) {
            case Qt::DisplayRole:
                return mObject->displayName();

            case Qt::DecorationRole:
                Heaven::IconRef iref = mObject->icon(true);
                Heaven::Icon icon = iref.icon();
                return icon.pixmap();
            }
        }
        return RefItem::data(role);
    }

protected:
    T* mObject;
};

class RefScope : public RefItemObject<RM::RefTreeNode>
{
public:
    enum { StaticType = Scope };

public:
    RefScope(RefItem* parent, RM::RefTreeNode* obj)
        : RefItemObject(parent, obj)
    {}

protected:
    ItemType type() const;
};


class RefNameSpace : public RefScope
{
public:
    enum { StaticType = Namespace };
public:
    RefNameSpace( RefItem* p, const QString& t );

    ItemType type() const;
};

class RefRemote : public RefItemObject<RM::Remote>
{
public:
    enum { StaticType = Remote };

public:
    RefRemote(RefItem* parent, RM::Remote* remote)
        : RefItemObject(parent, remote)
    {
    }

public:
    ItemType type() const;
};

class RefBranch : public RefItemObject<RM::Branch>
{
public:
    enum { StaticType = Branch };

public:
    RefBranch(RefItem* parent, RM::Branch* branch)
        : RefItemObject(parent, branch)
    {
    }

public:
    QVariant data(int role) const;
    ItemType type() const;
};


class RefTag : public RefItemObject<RM::Tag>
{
public:
    enum { StaticType = Tag };

public:
    RefTag(RefItem* parent, RM::Tag* tag)
        : RefItemObject(parent, tag)
    {
    }

public:
    QVariant data(int role) const;
    ItemType type() const;
};
