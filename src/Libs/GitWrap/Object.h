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

#ifndef GIT_OBJECT_H
#define GIT_OBJECT_H

#include "Git.h"
#include "ObjectId.h"

namespace Git
{

	class ObjectPrivate;

	class ObjectTree;
	class ObjectBlob;
	class ObjectCommit;
	class ObjectTag;

	class Repository;

    /**
     * @brief The Object class is the base class for Git repository objects.
     *
     * @ingroup GitWrap
     * @{
     */
	class GITWRAP_API Object
	{
	public:
		Object( ObjectPrivate* _d );
		Object( const Object& other );
		Object();
		~Object();

	public:
		Object& operator=( const Object& other );
		bool operator==( const Object& other ) const;
		bool isValid() const;

		ObjectType type() const;
		ObjectId id() const;

		ObjectTree asTree();
		ObjectCommit asCommit();
		ObjectBlob asBlob();
		ObjectTag asTag();

		bool isTree() const;
		bool isTag() const;
		bool isCommit() const;
		bool isBlob() const;

		Repository repository() const;

	protected:
		GitPtr< ObjectPrivate > d;
	};

    /**@}*/
}

#endif
