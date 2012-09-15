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

#ifndef GIT_REMOTE_PRIVATE_H
#define GIT_REMOTE_PRIVATE_H

#include "GitWrapPrivate.h"
#include "RepoObject.h"

namespace Git
{

	BEGIN_INTERNAL_DECL()

    /**
	 * @internal
	 * @ingroup GitWrap
     * @brief The RemotePrivate class
     *
     */
	class RemotePrivate : public RepoObject
	{
	public:
		RemotePrivate( RepositoryPrivate* repo, git_remote* remote );
		~RemotePrivate();

	public:
		git_remote*			mRemote;
		git_off_t			mBytes;
		git_indexer_stats	mStats;
	};

	END_INTERNAL_DECL()

}

#endif
