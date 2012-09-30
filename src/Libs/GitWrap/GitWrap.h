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

#ifndef GITWRAP_GITWRAP_H
#define GITWRAP_GITWRAP_H

#include <QString>
#include <QHash>
#include <QMetaType>

/** @defgroup GitWrap Qt-based wrapper for Git revision control featuring libgit2
 * Wrapper for the LibGit2 library based on Qt4/Qt5.
 */

#ifdef GitWrap_EXPORTS
#	define GITWRAP_API Q_DECL_EXPORT
#else
#	define GITWRAP_API Q_DECL_IMPORT
#endif

#define GITWRAP_DEFAULT_TLSRESULT /* Nothing */

namespace Git
{

	namespace Internal
	{

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

	}

	enum ObjectType
	{
		otTree,
		otCommit,
		otBlob,
		otTag,

		otAny = -1
	};

	enum TreeEntryAttributes
	{
		UnkownAttr			= 0,
		TreeAttr			= 0040000,
		FileAttr			= 0100644,
		FileExecutableAttr	= 0100755,
		GitLinkAttr			= 0120000,
		SubmoduleAttr		= 0160000
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

	typedef QFlags< FileStatus > FileStati;

	typedef QHash< QString, FileStati > StatusHash;

	class Result;

	class GITWRAP_API GitWrap
	{
	public:
		GitWrap();
		~GitWrap();

	public:
		static Result& lastResult();
	};
}

#endif
