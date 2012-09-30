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

#ifndef GIT_TREE_BUILDER_H
#define GIT_TREE_BUILDER_H

#include "GitWrap.h"

namespace Git
{

	namespace Internal
	{
		class TreeBuilderPrivate;
	}

	class TreeEntry;
	class ObjectId;

	/**
	 * @ingroup		GitWrap
	 * @brief		Tool for creating tree objects
	 */
	class GITWRAP_API TreeBuilder
	{
	public:
		TreeBuilder();
		TreeBuilder( Internal::TreeBuilderPrivate* _d );
		TreeBuilder( const TreeBuilder& other );
		~TreeBuilder();
		TreeBuilder& operator=( const TreeBuilder& other );

	public:
		bool isValid() const;

		void clear( Result& result GITWRAP_DEFAULT_TLSRESULT );

		TreeEntry get( const QString& name, Result& result GITWRAP_DEFAULT_TLSRESULT );
		bool insert( const QString& fileName, TreeEntryAttributes type, const ObjectId& oid,
					 Result& result GITWRAP_DEFAULT_TLSRESULT );
		bool remove( const QString& fileName, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectId write( Result& result GITWRAP_DEFAULT_TLSRESULT );

	private:
		Internal::GitPtr< Internal::TreeBuilderPrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::TreeBuilder )

#endif
