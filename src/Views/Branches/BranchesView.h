
#ifndef MGV_BRANCHES_VIEW_H
#define MGV_BRANCHES_VIEW_H

#include "Libs/Heaven/HView.h"

#include "Libs/Git/Repository.h"

class QListWidget;
class QToolBar;
class QToolButton;

class BranchesView : public HeavenView
{
	Q_OBJECT
public:
	BranchesView();

private slots:
	void repositoryChanged( Git::Repository repo );
	void rereadBranches();

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
	QToolBar*			mToolBar;
	QToolButton*		mBtnLocals;
	QToolButton*		mBtnRemotes;
};

#endif
