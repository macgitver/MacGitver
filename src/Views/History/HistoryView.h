
#ifndef MGV_HISTORY_VIEW_H
#define MGV_HISTORY_VIEW_H

#include <QWidget>
#include <QItemDelegate>

class QToolBar;

#include "Libs/Git/Git.h"
#include "Libs/Git/Repository.h"

#include "Libs/Heaven/HView.h"

class HistoryBuilder;
class HistoryModel;
class HistoryList;

class HistoryViewDelegate : public QItemDelegate
{
public:
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

class HistoryView : public HeavenView
{
	Q_OBJECT
public:
	HistoryView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	void buildHistory();

private:
	HistoryModel*		mModel;
	HistoryList*		mList;
	HistoryBuilder*		mBuilder;
	QToolBar*			mToolBar;
	Git::Repository		mRepo;
};

#endif

