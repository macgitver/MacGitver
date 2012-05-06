
#ifndef INDEX_TREE_H
#define INDEX_TREE_H

#include <QTreeWidget>

#include "Core/Index.h"
#include "Core/Repository.h"

class WorkingTreeDirNode;

class WorkingTreeNode
{
public:
	WorkingTreeNode( const QString& path, QTreeWidgetItem* item );

public:
	QString path(){ return mPath; }
	QTreeWidgetItem* item(){ return mItem; }

private:
	QString mPath;
	QTreeWidgetItem* mItem;
};

class WorkingTreeFileNode : public WorkingTreeNode
{
public:
	WorkingTreeFileNode( const QString& path, QTreeWidgetItem* item );
};

class WorkingTreeDirNode : public WorkingTreeNode
{
public:
	WorkingTreeDirNode( const QString& path, QTreeWidgetItem* item, bool deleteItem = true );
	~WorkingTreeDirNode();

private:
	bool mDeleteItem;
};

class IndexTree : public QTreeWidget
{
	Q_OBJECT
public:
	IndexTree();

public:
	void setRepository( const Git::Repository& repo );

private:
	void update();
	void clear();

private:
	Git::Repository							mRepo;
	Git::Index								mIndex;
	QHash< QString, WorkingTreeDirNode* >	mPathToNodes;
	QHash< QString, WorkingTreeFileNode* >	mFileToNodes;
};

#endif
