
#ifndef INDEX_WIDGET_H
#define INDEX_WIDGET_H

#include <QWidget>

class QToolBar;

#include "Libs/Git/Git.h"
#include "Libs/Git/Repository.h"

#include "Libs/Heaven/HView.h"

class IndexTree;

class IndexWidget : public HeavenView
{
	Q_OBJECT
public:
	IndexWidget();

private slots:
	void repositoryChanged( Git::Repository repo );

	void onShowAll( bool enabled );
	void onShowModified( bool enabled );
	void onShowMissing( bool enabled );
	void onShowIgnored( bool enabled );
	void onShowUntracked( bool enabled );
	void onShowUnchanged( bool enabled );

private:
	IndexTree*			mTree;
	QToolBar*			mToolBar;
	Git::Repository		mRepo;

	bool				mChangingFilters;

	QAction*			mactShowAll;
	QAction*			mactShowUnchanged;
	QAction*			mactShowModified;
	QAction*			mactShowMissing;
	QAction*			mactShowIgnored;
	QAction*			mactShowUntacked;
};

#endif

