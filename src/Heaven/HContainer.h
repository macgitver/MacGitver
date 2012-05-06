
#ifndef HEAVEN_CONTAINER_H
#define HEAVEN_CONTAINER_H

#include <QObject>
#include <QTableWidget>
#include <QSplitter>

typedef QTabWidget HeavenTabWidget;
typedef QSplitter HeavenSplitter;

class HeavenView;

class HeavenContainer : public QObject
{
	Q_OBJECT
public:
	enum Type
	{
		Tab			= 1 << 8,
		Splitter	= 1 << 9,
		BaseMask	= Tab|Splitter,

		SubTabTop		= 0,
		SubTabBottom	= 1,
		SubTabRight		= 2,
		SubTabLeft		= 3,

		SubSplitVert	= 0,
		SubSplitHorz	= 1,

		SubMask			= 255
	};

	struct Content
	{
		bool					isView;
		union
		{
			HeavenView*			view;
			HeavenContainer*	container;
		};
	};

public:
	HeavenContainer( Type t, Type s, HeavenContainer* parent = NULL );
	~HeavenContainer();


public:
	Type type() const;

	QList< HeavenView* > views() const;
	int numViews() const;
	int addView( HeavenView* view );
	HeavenView* takeView( int index );
	QWidget* containerWidget();

	QList< HeavenContainer* > containers() const;
	int numContainers() const;
	int addContainer( HeavenContainer* container );
	void insertContainer( int pos, HeavenContainer* container );

private:
	Type				mType;
	QList< Content >	mContent;

	union
	{
		QWidget*			mContainerWidget;
		HeavenSplitter*		mSpliterWidget;
		HeavenTabWidget*	mTabWidget;
	};
};

#endif
