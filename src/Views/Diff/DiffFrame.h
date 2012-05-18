
#ifndef MGV_DIFF_VIEW_FRAME_H
#define MGV_DIFF_VIEW_FRAME_H

#include <QFrame>

class QScrollBar;

class Patch;

class DiffSplitter;
class DiffWidget;

class DiffFrame : public QFrame
{
	Q_OBJECT
public:
	DiffFrame();

public:
	void setPatch( Patch* patch );

private:
	DiffSplitter*			mSplitter;
	QScrollBar*				mScrollV;
	QScrollBar*				mScrollH;
	QList< DiffWidget* >	mWidgets;


	Patch*					mPatch;
};

#endif
