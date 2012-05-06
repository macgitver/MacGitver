
#ifndef MGV_HISTORY_VIEW_H
#define MGV_HISTORY_VIEW_H

#include <QWidget>

#include "Core/Git.h"
#include "Core/Repository.h"

#include "Heaven/HView.h"

class HistoryModel;
class HistoryList;

class HistoryView : public HeavenView
{
	Q_OBJECT
public:
	HistoryView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	HistoryModel*		mModel;
	HistoryList*		mList;
	Git::Repository		mRepo;
};

#endif

