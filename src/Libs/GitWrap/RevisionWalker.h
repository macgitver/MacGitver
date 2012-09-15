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

#ifndef GIT_REVISION_WALKER_H
#define GIT_REVISION_WALKER_H

#include "GitWrap.h"

namespace Git
{

	namespace Internal
	{
		class RevisionWalkerPrivate;
	}

	class ObjectId;
	class Reference;

	/**
	 * @ingroup		GitWrap
	 * @brief		Provides access to a git repository's history
	 *
	 */
	class GITWRAP_API RevisionWalker
	{
	public:
		RevisionWalker();
		RevisionWalker( Internal::RevisionWalkerPrivate* _d );
		~RevisionWalker();
		RevisionWalker& operator=( const RevisionWalker& other );

	public:
		bool isValid() const;

		void reset( Result& result = GitWrap::lastResult() );

		void push( const ObjectId& id, Result& result = GitWrap::lastResult()  );
		void push( const Reference& ref, Result& result = GitWrap::lastResult() );
		void pushRef( const QString& name, Result& result = GitWrap::lastResult() );
		void pushHead( Result& result = GitWrap::lastResult() );

		void hide( const ObjectId& id, Result& result = GitWrap::lastResult() );
		void hide( const Reference& ref, Result& result = GitWrap::lastResult() );
		void hideRef( const QString& name, Result& result = GitWrap::lastResult() );
		void hideHead( Result& result = GitWrap::lastResult() );

		bool next( ObjectId& oidNext, Result& result = GitWrap::lastResult() );
		QVector< ObjectId > all( Result& result = GitWrap::lastResult() );

		void setSorting( bool topological, bool timed, Result& result = GitWrap::lastResult() );

	private:
		Internal::GitPtr< Internal::RevisionWalkerPrivate > d;
	};

}

#endif
