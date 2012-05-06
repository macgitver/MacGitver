
#ifndef MGV_TAGS_VIEW_H
#define MGV_TAGS_VIEW_H

#include "Heaven/HView.h"

#include "Core/Repository.h"

class QListWidget;

class TagsView : public HeavenView
{
	Q_OBJECT
public:
	TagsView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
};

#endif
