/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GIT_DIFFLIST_PRIVATE_H
#define GIT_DIFFLIST_PRIVATE_H

#include "Git_p.h"
#include "RepoObject.h"

namespace Git
{

	class DiffListPrivate : public RepoObject
	{
	public:
		DiffListPrivate( RepositoryPrivate* repo, git_diff_list* difflist );
		~DiffListPrivate();

	public:
		git_diff_list*	mDiffList;
	};

}

#endif
