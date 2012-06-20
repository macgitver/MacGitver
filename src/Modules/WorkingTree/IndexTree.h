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

#ifndef INDEX_TREE_H
#define INDEX_TREE_H

#include <QTreeWidget>

#include "GitWrap/Index.h"
#include "GitWrap/Repository.h"

#include "WorkingTreeFilters.h"

class IndexTree;

class WorkingTreeDirNode;

class WorkingTreeNode
{
public:
	WorkingTreeNode( const QString& path, QTreeWidgetItem* item );
	virtual ~WorkingTreeNode();

public:
	QString path(){ return mPath; }
	QTreeWidgetItem* item(){ return mItem; }

	virtual bool refilter( IndexTree* tree ) = 0;

private:
	QString mPath;
	QTreeWidgetItem* mItem;
};

class WorkingTreeDirNode;
class WorkingTreeFileNode;

class IndexTree : public QTreeWidget
{
	Q_OBJECT
public:
	IndexTree();

public:
	void setRepository( const Git::Repository& repo );

	WorkingTreeFilters filters() const;
	void setFilter( WorkingTreeFilters filters );

private:
	void update();
	void clear();
	void refilter();

private:
	Git::Repository							mRepo;
	Git::Index								mIndex;
	QHash< QString, WorkingTreeDirNode* >	mPathToNodes;
	QHash< QString, WorkingTreeFileNode* >	mFileToNodes;
	WorkingTreeFilters						mFilters;
};

class WorkingTreeFileNode : public WorkingTreeNode
{
public:
	WorkingTreeFileNode( const QString& path, QTreeWidgetItem* item, WorkingTreeDirNode* parent,
						 WorkingTreeFilters state );

public:
	virtual bool refilter( IndexTree* tree );

private:
	WorkingTreeDirNode*	mParent;
	WorkingTreeFilters	mState;
};

class WorkingTreeDirNode : public WorkingTreeNode
{
public:
	WorkingTreeDirNode( const QString& path, QTreeWidgetItem* item, WorkingTreeDirNode* parent );
	~WorkingTreeDirNode();

public:
	virtual bool refilter( IndexTree* tree );

private:
	WorkingTreeDirNode* mParent;
public:
	QList< WorkingTreeNode* > mChildren;
};

#endif
