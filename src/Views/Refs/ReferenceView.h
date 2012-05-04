
#ifndef MGV_REFERENCE_VIEW_H
#define MGV_REFERENCE_VIEW_H

#include <QWidget>

#include "Core/Repository.h"

class QListWidget;

class ReferenceView : public QWidget
{
	Q_OBJECT
public:
	ReferenceView();

private slots:
	void repositoryChanged( Git::Repository repo );

private:
	Git::Repository		mRepo;
	QListWidget*		mListWidget;
};

#endif
