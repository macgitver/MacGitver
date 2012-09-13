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

#include "Index.h"
#include "Repository.h"

namespace Git
{

	namespace Internal
	{

		GitWrapPrivate* GitWrapPrivate::self = NULL;

	}

	GitWrap::GitWrap()
	{
		git_threads_init();

		Q_ASSERT( Internal::GitWrapPrivate::self == NULL );
		Internal::GitWrapPrivate::self = new Internal::GitWrapPrivate;
	}

	GitWrap::~GitWrap()
	{
		Q_ASSERT( Internal::GitWrapPrivate::self != NULL );
		delete Internal::GitWrapPrivate::self;
		Internal::GitWrapPrivate::self = NULL;

		git_threads_shutdown();
	}

	Result& GitWrap::lastResult()
	{
		Q_ASSERT( Internal::GitWrapPrivate::self != NULL );

		if( !Internal::GitWrapPrivate::self->mTLStore.hasLocalData() )
		{
			Internal::GitWrapTLS* tls = new Internal::GitWrapTLS;
			Internal::GitWrapPrivate::self->mTLStore.setLocalData( tls );
		}

		Internal::GitWrapTLS* tls = Internal::GitWrapPrivate::self->mTLStore.localData();
		return tls->mLastResult;
	}

}
