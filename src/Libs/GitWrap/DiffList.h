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

#ifndef GIT_DIFFLIST_H
#define GIT_DIFFLIST_H

#include "Git.h"

namespace Git
{

	class ChangeListConsumer;
	class PatchConsumer;

	class DiffListPrivate;

    /**
     * @brief The DiffList class
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API DiffList
	{
	public:
		explicit DiffList( DiffListPrivate* _d );

	public:
		DiffList( const DiffList& other );
		DiffList();
		~DiffList();

	public:
		DiffList& operator=( const DiffList& other );

	public:
		bool isValid() const;
		Repository repository() const;

		bool mergeOnto( DiffList other ) const;

		bool consumePatch( PatchConsumer* consumer ) const;
		bool consumeChangeList( ChangeListConsumer* consumer ) const;

	private:
		GitPtr< DiffListPrivate > d;
	};

    /**@}*/
}

#endif
