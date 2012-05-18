
#ifndef DIFF_VIEW_H
#define DIFF_VIEW_H

#include <QWidget>

#include "Libs/Heaven/HView.h"

class QTreeWidget;
class QStackedWidget;
class QTextBrowser;
class QScrollBar;

class Patch;
class DiffWidget;
class DiffTreeFileList;
class DiffFrame;

class DiffView : public HeavenView
{
	Q_OBJECT
public:
	DiffView();
	~DiffView();

public:
	void setPatch( Patch* patch );

private:
	void clearTree();
	void fillTree();

private:
	Patch*				mPatch;
	DiffTreeFileList*	mTree;
	QTreeWidget*		mDetails;
	QStackedWidget*		mDiffStack;
	QTextBrowser*		mRawDiffView;
	DiffFrame*			mDiffFrame;
	QScrollBar*			mScrollV;
	QScrollBar*			mScrollH;
};

#endif
