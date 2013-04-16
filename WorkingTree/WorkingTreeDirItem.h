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

#ifndef MGV_WORKING_TREE_DIR_ITEM_H
#define MGV_WORKING_TREE_DIR_ITEM_H

#include <QList>
#include <QHash>
#include <QIcon>

#include "WorkingTreeAbstractItem.h"

class WorkingTreeFileItem;

class WorkingTreeDirItem : public WorkingTreeAbstractItem
{
public:
    WorkingTreeDirItem( WorkingTreeModel* model, WorkingTreeAbstractItem* parent );
    ~WorkingTreeDirItem();

public:
    void setName( const QString& name );
    void setIcon( const QIcon& icon );

public:
    QString name() const;
    bool isDirectory() const;
    int childCount() const;
    WorkingTreeAbstractItem* childAt( int index );
    QVariant data( int column, int role ) const;
    int row() const;
    WorkingTreeAbstractItem* childByName( const QString& name );
    void removeChild( WorkingTreeAbstractItem* child );
    void appendItem( WorkingTreeAbstractItem* item );

    friend class WorkingTreeAbstractItem;
private:
    int totalChildren() const{ return mChildren.count(); }

private:
    QString                                     mName;
    QIcon                                       mIcon;
    QVector< WorkingTreeDirItem* >              mDirs;
    QVector< WorkingTreeFileItem* >             mFiles;
    QHash< QString, WorkingTreeAbstractItem* >  mChildren;
};

#endif
