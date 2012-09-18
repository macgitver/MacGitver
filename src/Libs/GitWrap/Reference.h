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

#ifndef GIT_REFERENCE_H
#define GIT_REFERENCE_H

#include "GitWrap.h"

namespace Git
{

	class ObjectId;
	class Repository;

	namespace Internal
	{
		class ReferencePrivate;
	}

	/**
	 * @ingroup		GitWrap
	 * @brief		Represents a git reference
	 *
	 */
	class GITWRAP_API Reference
	{
	public:
		enum Type
		{
			Direct, Symbolic, Invalid = -1
		};

	public:
		Reference();
		Reference( Internal::ReferencePrivate* p );
		Reference( const Reference& other );
		~Reference();
		Reference& operator=( const Reference& other );

	public:
		bool isValid() const;
		bool destroy( Result& result = GitWrap::lastResult() );
		QString name() const;

		Type type( Result& result = GitWrap::lastResult() ) const;
		ObjectId objectId( Result& result = GitWrap::lastResult() ) const;
		QString target( Result& result = GitWrap::lastResult() ) const;

		Repository repository( Result& result = GitWrap::lastResult() ) const;

	private:
		Internal::GitPtr< Internal::ReferencePrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::Reference )

#endif
