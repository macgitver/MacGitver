
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();


private slots:
	void onRepositoryCreate();

private:
	void setupUi();

public:
	void addError( const QString& err );

private:
	QMenu*			mmuRepository;
	QAction*		macRepositoryOpen;
	QAction*		macRepositoryClose;
	QAction*		macRepositoryCreate;
	QAction*		macRepositoryClone;
	QAction*		macRepositoryQuit;
	static MainWindow* sSelf;
public:
	static MainWindow& self(){ return *sSelf; }
};

#endif
