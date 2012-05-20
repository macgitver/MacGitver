
#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QApplication>

#include "Libs/Git/Repository.h"

class Module;
class Modules;
class MainWindow;

class MacGitver : public QApplication
{
	Q_OBJECT
public:
	MacGitver( int argc, char **argv );
	~MacGitver();

public:
	static MacGitver& self();

public:
	void closeRepository();
	void openedRepository( const Git::Repository &repo );
	Git::Repository repository() const;
	Modules* modules();

	MainWindow* mainWindow();

signals:
	void repositoryChanged( const Git::Repository& repo );

private:
	void setRepository( const Git::Repository &repo );

private:
	static MacGitver*	sSelf;

	Modules*			mModules;
	MainWindow*			mMainWindow;
	Git::Repository		mRepository;
};

#endif

