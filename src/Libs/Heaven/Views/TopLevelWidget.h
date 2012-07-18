
#ifndef HEAVEN_TOP_LEVEL_WIDGET_H
#define HEAVEN_TOP_LEVEL_WIDGET_H

#include <QWidget>

#include "Heaven/HeavenApi.h"

#include "Heaven/Views/Container.h"
#include "Heaven/Views/View.h"

namespace Heaven
{

	class HEAVEN_API TopLevelWidget : public QWidget
	{
		Q_OBJECT
	public:
		TopLevelWidget();
		~TopLevelWidget();

	public:
		void clear();

	public:
		ViewContainer* rootContainer();
		void setRootContainer( ViewContainer* c );

	public:
		void addContainer( ViewContainer* c );
		void addView( View* c, Positions pos = Left );

		QSet< View* > setOfViews() const;

	protected:
		void paintEvent( QPaintEvent* ev );

	private:
		ViewContainer*		mRoot;
	};

}

#endif
