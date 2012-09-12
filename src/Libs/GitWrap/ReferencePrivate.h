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

#ifndef GIT_REFERENCE_PRIVATE_H
#define GIT_REFERENCE_PRIVATE_H

#include "RepoObject.h"

namespace Git
{

	BEGIN_INTERNAL_DECL()

    /**
	 * @ingroup		GitWrap
	 * @brief		The ReferencePrivate class
     *
     */
	class ReferencePrivate : public RepoObject
	{
	public:
		ReferencePrivate( RepositoryPrivate* repo, git_reference* ref );
		~ReferencePrivate();

	public:
		git_reference*		mRef;
	};

	END_INTERNAL_DECL()

}

#endif
