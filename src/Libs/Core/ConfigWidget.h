
#ifndef MGV_CONFIG_WIDGET_H
#define MGV_CONFIG_WIDGET_H

#include <QWidget>
#include <QHash>
#include <QByteArray>

class QSplitter;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;

class ConfigPageGroup;
class ConfigPage;

class ConfigWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigWidget();

public:

private:
	QStackedWidget*							mConfigContainer;
	QTreeWidget*							mPagesTree;
	QHash< ConfigPageGroup*, QTreeWidgetItem* >	mGroupItems;
	QHash< QTreeWidgetItem*, ConfigPage* >		mItemPages;
};

#endif
