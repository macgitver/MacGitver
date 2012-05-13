
#include "Libs/Git/Git_p.h"

#include "Libs/Git/Index.h"
#include "Libs/Git/Repository.h"

namespace Git
{

	// INDEX

	// tools

	void initLibGit()
	{
		git_threads_init();
	}

	void deinitLibGit()
	{
		git_threads_shutdown();
	}

}
