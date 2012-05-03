
#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include <QWidget>

#include "Core/Git.h"

class IndexTree;

class IndexWidget : public QWidget
{
	Q_OBJECT
public:
	IndexWidget();

private slots:
	void repositoryChanged( Git::Repository* repo );

private:
	IndexTree*			mTree;
	Git::Repository*	mRepo;
};

#endif

