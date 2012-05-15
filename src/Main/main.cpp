
#include "Libs/Core/MacGitver.h"

int main( int c, char** v )
{
	MacGitver mgv( c, v );
	return mgv.exec();
}
