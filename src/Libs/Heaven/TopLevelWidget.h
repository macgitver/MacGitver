
#ifndef HEAVEN_TOP_LEVEL_WIDGET_H
#define HEAVEN_TOP_LEVEL_WIDGET_H

#include <QWidget>

#include "Heaven/HeavenApi.h"

#include "Heaven/Container.h"
#include "Heaven/View.h"

class HEAVEN_API HeavenTopLevelWidget : public QWidget
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
