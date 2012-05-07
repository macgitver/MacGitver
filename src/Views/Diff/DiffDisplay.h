
#ifndef DIFF_DISPLAY_H
#define DIFF_DISPLAY_H


#include <QFont>
#include <QAbstractScrollArea>

class Difference;

class DiffDisplayWidget : public QAbstractScrollArea
{
	Q_OBJECT
public:
	DiffDisplayWidget();
	~DiffDisplayWidget();

public:
	void setDifference( Difference* diff );

protected:
	void paintEvent( QPaintEvent* ev );
	void resizeEvent( QResizeEvent* ev );

private:
	void calculateHeights();

private:
	Difference*			mDiff;
	QFont				mFont;
	int					mTotalHeight;
};

#endif
