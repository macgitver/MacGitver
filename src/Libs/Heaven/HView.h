
#ifndef HEAVEN_VIEW_H
#define HEAVEN_VIEW_H

#include <QWidget>

class HeavenView : public QWidget
{
	Q_OBJECT
public:
	enum Type
	{
		Multiviewable,
		Singleviewable
	};


public:
	HeavenView();

public:
	QString viewName() const;
	void setViewName( const QString& name );

signals:
	void nameChanged( const QString& viewName );

private:
	QString				mViewName;
	QAction*			mAction;
};

#endif
