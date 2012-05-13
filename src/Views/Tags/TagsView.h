
#ifndef MGV_TAGS_VIEW_H
#define MGV_TAGS_VIEW_H

#include "Libs/Heaven/HView.h"

#include "Libs/Git/Repository.h"

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
