
#ifndef MGV_MGV_MAIN_H
#define MGV_MGV_MAIN_H

#include "MacGitver/MacGitver.h"

class MacGitverMain : public MacGitver
{
public:
	MacGitverMain( int argc, char** argv );

public:
	int exec();
};

#endif
