
#ifndef DIFF_SPLITTER_H
#define DIFF_SPLITTER_H

#include <QSplitter>

class DiffSplitter : public QSplitter
{
	Q_OBJECT
public:
	DiffSplitter( QWidget* parent = 0 );

protected:
	virtual QSplitterHandle* createHandle();

private:
};

class DiffSplitterHandle : public QSplitterHandle
{
public:
	DiffSplitterHandle( Qt::Orientation o, DiffSplitter *parent );

protected:
	void paintEvent( QPaintEvent* ev );

private:
};

#endif
