
#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include <QWidget>

#include "Core/Git.h"
#include "Core/Repository.h"

#include "Heaven/HView.h"

class IndexTree;

class IndexWidget : public HeavenView
{
	Q_OBJECT
public:
	IndexWidget();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	IndexTree*			mTree;
	Git::Repository		mRepo;
};

#endif

