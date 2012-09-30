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

#include "GitWrap.h"
#include "ObjectId.h"

namespace Git
{

	namespace Internal
	{
		class ObjectPrivate;
	}

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
		Object( Internal::ObjectPrivate* _d );
		Object( const Object& other );
		Object();
		~Object();

	public:
		Object& operator=( const Object& other );
		bool operator==( const Object& other ) const;

		/**
		 * @brief Checks, if this is a valid Git object.
		 *
		 * A Git object is valid, when it has a valid pointer.
		 *
		 * @return true or false
		 */
		bool isValid() const;

		/**
		 * @return the object's type
		 */
		ObjectType type( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @return the object's id (OID)
		 */
		ObjectId id( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @brief Converts a generic object into a Git tree object.
		 *
		 * @return the valid or invalid converted ObjectTree
		 *
		 * @see isValid()
		 */
		ObjectTree asTree( Result& result GITWRAP_DEFAULT_TLSRESULT );

		/**
		 * @brief Converts a generic object into a Git commit object.
		 *
		 * @return the valid or invalid converted ObjectCommit
		 *
		 * @see isValid()
		 */
		ObjectCommit asCommit( Result& result GITWRAP_DEFAULT_TLSRESULT );

		/**
		 * @brief Converts a generic object into a Git BLOB object.
		 *
		 * @return the valid or invalid converted ObjectBlob
		 *
		 * @see isValid()
		 */
		ObjectBlob asBlob( Result& result GITWRAP_DEFAULT_TLSRESULT );

		/**
		 * @brief Converts a generic object into a Git tag object.
		 *
		 * @return the valid or invalid converted ObjectTag
		 *
		 * @see isValid()
		 */
		ObjectTag asTag( Result& result GITWRAP_DEFAULT_TLSRESULT );

		/**
		 * @brief Checks, if this is a ObjectTree object.
		 * @return true or false
		 */
		bool isTree( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @brief Checks, if this is a ObjectTree object.
		 * @return true or false
		 */
		bool isTag( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @brief Checks, if this is a ObjectTree object.
		 * @return true or false
		 */
		bool isCommit( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @brief Checks, if this is a ObjectTree object.
		 * @return true or false
		 */
		bool isBlob( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		/**
		 * @return the owner repository or an invalid repository
		 */
		Repository repository( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

	protected:
		Internal::GitPtr< Internal::ObjectPrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::Object )

#endif
