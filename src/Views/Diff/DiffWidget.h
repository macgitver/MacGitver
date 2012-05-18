
#ifndef DIFF_DISPLAY_H
#define DIFF_DISPLAY_H

#include <QFont>
#include <QWidget>

class PatchFile;

class DiffWidget : public QWidget
{
	Q_OBJECT
public:
	DiffWidget();
	~DiffWidget();

public:
	void setDifference( PatchFile* diff );

protected:
	void paintEvent( QPaintEvent* ev );
	void resizeEvent( QResizeEvent* ev );

private:
	void calculateHeights();

private:
	PatchFile*			mDiff;
	QFont				mFont;
	int					mTotalHeight;
};

#endif
