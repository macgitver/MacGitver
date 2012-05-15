
#ifndef MGV_MACGITVER_H
#define MGV_MACGITVER_H

#include <QApplication>

class MacGitver : public QApplication
{
	Q_OBJECT
public:
	MacGitver( int argc, char **argv );
	~MacGitver();

public:
	static MacGitver& self();

private:
	static MacGitver* sSelf;
};

#endif

