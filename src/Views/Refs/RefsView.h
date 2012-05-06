
#ifndef MGV_REFERENCE_VIEW_H
#define MGV_REFERENCE_VIEW_H

#include "Heaven/HView.h"

#include "Core/Repository.h"

class QListWidget;

class RefsView : public HeavenView
{
	Q_OBJECT
public:
	RefsView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
};

#endif
