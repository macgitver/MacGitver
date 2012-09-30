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

		void reset( Result& result GITWRAP_DEFAULT_TLSRESULT );

		void push( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT  );
		void push( const Reference& ref, Result& result GITWRAP_DEFAULT_TLSRESULT );
		void pushRef( const QString& name, Result& result GITWRAP_DEFAULT_TLSRESULT );
		void pushHead( Result& result GITWRAP_DEFAULT_TLSRESULT );

		void hide( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT );
		void hide( const Reference& ref, Result& result GITWRAP_DEFAULT_TLSRESULT );
		void hideRef( const QString& name, Result& result GITWRAP_DEFAULT_TLSRESULT );
		void hideHead( Result& result GITWRAP_DEFAULT_TLSRESULT );

		bool next( ObjectId& oidNext, Result& result GITWRAP_DEFAULT_TLSRESULT );
		QVector< ObjectId > all( Result& result GITWRAP_DEFAULT_TLSRESULT );

		void setSorting( bool topological, bool timed, Result& result GITWRAP_DEFAULT_TLSRESULT );

	private:
		Internal::GitPtr< Internal::RevisionWalkerPrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::RevisionWalker )

#endif
