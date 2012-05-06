
#ifndef HEAVEN_DECORATOR_H
#define HEAVEN_DECORATOR_H

#include <QWidget>

class HeavenView;

class HeavenDecorator : public QWidget
{
	Q_OBJECT
public:
	HeavenDecorator( HeavenView* view );

private:
	void paintEvent( QPaintEvent* ev );

private:
	HeavenView*		mView;
};

#endif
