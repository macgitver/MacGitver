
#ifndef MGV_REFERENCE_VIEW_H
#define MGV_REFERENCE_VIEW_H

#include "Heaven/HView.h"

#include "Core/Repository.h"

class QListWidget;

class ReferenceView : public HeavenView
{
	Q_OBJECT
public:
	ReferenceView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
};

#endif
