
#ifndef INCLUDE_git_checkout_h__
#define INCLUDE_git_checkout_h__

#include "git2.h"

GIT_EXTERN(int) git_workdir_init(
	git_repository* repo );

GIT_EXTERN(int) git_workdir() ;

GIT_EXTERN(int) git_workdir_set_branch(
	git_repository* repo,
	const char* branchName );

#endif
