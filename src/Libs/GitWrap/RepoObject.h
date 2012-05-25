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

#ifndef GIT_REPO_OBJECT_H
#define GIT_REPO_OBJECT_H

#include <QAtomicInt>

namespace Git
{
	class RepositoryPrivate;

	class RepoObject
	{
	public:
		RepoObject( RepositoryPrivate* repo );
		virtual ~RepoObject();

	public:
		void ref();
		void deref();

		RepositoryPrivate* repo() const;
		bool handleErrors( int rc ) const;

	protected:
		RepositoryPrivate*	mRepo;
	private:
		QAtomicInt			mRefCounter;
	};

}

#endif

