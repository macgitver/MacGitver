/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#include "Git.h"

namespace Git
{

	class IndexPrivate;

	class GITWRAP_API Index
	{
	public:
		explicit Index( IndexPrivate* _d );

	public:
		Index( const Index& other );
		Index();
		~Index();

	public:
		Index& operator=( const Index& other );

	public:
		bool isValid() const;
		int count() const;

		Repository repository() const;

	private:
		GitPtr< IndexPrivate > d;
	};

}

#endif
