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

#ifndef GIT_INDEX_H
#define GIT_INDEX_H

#include "GitWrap.h"

namespace Git
{

	class Repository;

	namespace Internal
	{
		class IndexPrivate;
	}

	/**
	 * @ingroup		GitWrap
	 * @brief		Provides access to the git index.
	 *
	 */
	class GITWRAP_API Index
	{
	public:
		explicit Index( Internal::IndexPrivate* _d );

	public:
		Index( const Index& other );
		Index();
		~Index();

	public:
		Index& operator=( const Index& other );

	public:
		bool isValid() const;

		void read( Result& result GITWRAP_DEFAULT_TLSRESULT );
		void write( Result& result GITWRAP_DEFAULT_TLSRESULT );

		int count( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		Repository repository( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

	private:
		Internal::GitPtr< Internal::IndexPrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::Index )

#endif
