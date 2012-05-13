
#ifndef INDEX_TREE_H
#define INDEX_TREE_H

#include <QTreeWidget>

#include "Core/Index.h"
#include "Core/Repository.h"

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
	enum TreeFilter
	{
		Untracked		= 1 << 0,
		Missing			= 1 << 1,
		Changed			= 1 << 2,
		Unchanged		= 1 << 3,
		Ignored			= 1 << 4,

		All				= Unchanged | Untracked | Missing | Ignored | Changed
	};

	typedef QFlags< TreeFilter > TreeFilters;

public:
	IndexTree();

public:
	void setRepository( const Git::Repository& repo );

	TreeFilters filters() const;
	void setFilter( TreeFilters filters );

private:
	void update();
	void clear();
	void refilter();

private:
	Git::Repository							mRepo;
	Git::Index								mIndex;
	QHash< QString, WorkingTreeDirNode* >	mPathToNodes;
	QHash< QString, WorkingTreeFileNode* >	mFileToNodes;
	TreeFilters								mFilters;
};

class WorkingTreeFileNode : public WorkingTreeNode
{
public:
	WorkingTreeFileNode( const QString& path, QTreeWidgetItem* item, WorkingTreeDirNode* parent,
						 IndexTree::TreeFilters state );

public:
	virtual bool refilter( IndexTree* tree );

private:
	WorkingTreeDirNode*		mParent;
	IndexTree::TreeFilters	mState;
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
