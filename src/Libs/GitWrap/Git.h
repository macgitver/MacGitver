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

#ifndef CORE_GIT_H
#define CORE_GIT_H

#include <QString>
#include <QHash>

#ifdef GitWrap_EXPORTS
#	define GITWRAP_API Q_DECL_EXPORT
#else
#	define GITWRAP_API Q_DECL_IMPORT
#endif

namespace Git
{

	GITWRAP_API void initLibGit();
	GITWRAP_API void deinitLibGit();

	class Repository;
	class Index;

	template< class T >
	class GitPtr
	{
	public:
		GitPtr();
		GitPtr( const GitPtr< T >& o );
		GitPtr( T* o );
		~GitPtr();

		GitPtr< T >& operator=( const GitPtr< T >& o );
		bool operator==( const GitPtr< T >& o ) const;
		bool operator==( T* o ) const;

		T* operator->();
		const T* operator->() const;

		T* operator*();
		const T* operator*() const;

		operator bool() const;
		operator T*();
		operator const T*() const;

	private:
		T* d;
	};

	enum ObjectType
	{
		otTree,
		otCommit,
		otBlob,
		otTag,

		otAny = -1
	};

	enum FileStatus	// These are 1:1 to libgit2 for now
	{
		StatusCurrent				= 0,

		StatusIndexNew				= (1 << 0),
		StatusIndexModified			= (1 << 1),
		StatusIndexDeleted			= (1 << 2),

		StatusWorkingTreeNew		= (1 << 3),
		StatusWorkingTreeModified	= (1 << 4),
		StatusWorkingTreeDeleted	= (1 << 5),

		StatusIgnored				= (1 << 6)
	};

	typedef QHash< QString, FileStatus > StatusHash;

}

#endif
