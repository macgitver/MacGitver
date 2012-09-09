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

#ifndef GIT_REPOSITORY_PRIVATE_H
#define GIT_REPOSITORY_PRIVATE_H

#include <QMutex>

#include "Git_p.h"

namespace Git
{

	class Error;

    /**
     * @brief The RepositoryPrivate class
     *
     * @ingroup GitWrap
     * @{
     */
	class RepositoryPrivate
	{
	public:
		RepositoryPrivate( git_repository* repo );
		~RepositoryPrivate();

	public:
		void ref();
		void deref();

	public:
		bool handleErrors( int rc ) const;

	public:
		git_repository*			mRepo;
		IndexPrivate*			mIndex;
		mutable QList< Error >	mErrors;
		mutable QMutex			mErrorListMtx;

	private:
		QAtomicInt				mRefCounter;

		friend int status_callback( const char* name, unsigned int status, void* );
	};

    /**@}*/
}

#endif
