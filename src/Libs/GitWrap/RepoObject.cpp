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

#include "GitWrapPrivate.h"
#include "RepositoryPrivate.h"
#include "RepoObject.h"

namespace Git
{

	namespace Internal
	{

		BasicObject::BasicObject()
		{
		}

		BasicObject::~BasicObject()
		{
		}

		void BasicObject::ref()
		{
			mRefCounter.ref();
		}

		void BasicObject::deref()
		{
			if( !mRefCounter.deref() )
			{
				delete this;
			}
		}

		RepoObject::RepoObject( RepositoryPrivate* repo )
			: mRepo( repo )
		{
			Q_ASSERT( mRepo );
			if( mRepo )
			{
				mRepo->ref();
			}
		}

		RepoObject::~RepoObject()
		{
			if( mRepo )
			{
				mRepo->deref();
				mRepo = NULL;
			}
		}

		RepositoryPrivate* RepoObject::repo() const
		{
			return mRepo;
		}

		bool RepoObject::handleErrors( int rc ) const
		{
			return mRepo->handleErrors( rc );
		}

	}

}
