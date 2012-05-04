
#ifndef INDEX_TREE_H
#define INDEX_TREE_H

#include <QTreeWidget>

#include "Core/Index.h"

class IndexTree : public QTreeWidget
{
	Q_OBJECT
public:
	IndexTree();

public:
	void setIndex( const Git::Index& index );

private:
	Git::Index		mIndex;
};

#endif
