
#ifndef HEAVEN_TOP_LEVEL_WIDGET_H
#define HEAVEN_TOP_LEVEL_WIDGET_H

#include <QWidget>

#include "Heaven/HContainer.h"
#include "Heaven/HView.h"

struct Heaven
{

	enum Positions
	{
		Left,
		Right,
		Top,
		Bottom,
		Central
	};

};

class HeavenTopLevelWidget : public QWidget
{
	Q_OBJECT
public:
	HeavenTopLevelWidget();
	~HeavenTopLevelWidget();

public:
	void addContainer( HeavenContainer* c );
	void addView( HeavenView* c, Heaven::Positions pos = Heaven::Left );

protected:
	void paintEvent( QPaintEvent* ev );

private:
	HeavenContainer*		mRoot1;
	HeavenContainer*		mRoot2;
	HeavenContainer*		mDocks[ 5 ];
};

#endif
