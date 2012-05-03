
#include "Git_P.h"

#include "Core/Index.h"
#include "Core/Repository.h"

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
