
#ifndef DIFF_VIEW_H
#define DIFF_VIEW_H

#include "Heaven/HView.h"

class Differences;
class DiffDisplayWidget;
class DiffTreeFileList;

class DiffView : public HeavenView
{
	Q_OBJECT
public:
	DiffView();
	~DiffView();


private:
	Differences*		mPatch;
	DiffDisplayWidget*	mDisplay;
	DiffTreeFileList*	mTree;
};

#endif
