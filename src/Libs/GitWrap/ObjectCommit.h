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

#ifndef GIT_OBJECT_COMMIT_H
#define GIT_OBJECT_COMMIT_H

#include <QDebug>
#include <QList>

#include "GitWrap.h"
#include "ObjectId.h"
#include "Object.h"
#include "Signature.h"
#include "DiffList.h"
#include "Result.h"

namespace Git
{

	class Reference;
	class ObjectTree;

	/**
	 * @ingroup		GitWrap
	 * @brief		Represents a git commit object.
	 *
	 */
	class GITWRAP_API ObjectCommit : public Object
	{
	public:
		ObjectCommit();
		ObjectCommit( Internal::ObjectPrivate* _d );
		ObjectCommit( const ObjectCommit& o );

	public:
		bool operator==( const Git::ObjectCommit& commit ) const
		{
			Result r;
			return isEqual( commit, r ) && r;
		}

		bool operator!=( const Git::ObjectCommit& commit ) const
		{
			Result r;
			return !isEqual( commit, r ) && r;
		}

	public:
		ObjectTree tree( Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectId treeId( Result& result GITWRAP_DEFAULT_TLSRESULT );

		ObjectIdList parentCommitIds( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		QList< ObjectCommit > parentCommits( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		ObjectCommit parentCommit( unsigned int index, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		ObjectId parentCommitId( unsigned int index, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		unsigned int numParentCommits() const;

		bool isParentOf( const Git::ObjectCommit& child, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		bool isChildOf( const Git::ObjectCommit& parent, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		bool isEqual( const Git::ObjectCommit& commit, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		Signature author( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		Signature committer( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		QString message( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		QString shortMessage( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		Reference createBranch( const QString& name, bool force,
								Result& result GITWRAP_DEFAULT_TLSRESULT );

		DiffList diffFromParent( unsigned int index, Result& result GITWRAP_DEFAULT_TLSRESULT );
		DiffList diffFromAllParents( Result& result GITWRAP_DEFAULT_TLSRESULT );
	};

	/**
	 * @ingroup		GitWrap
	 * @brief		qHash() for Git::ObjectCommit
	 */
	inline uint qHash( const ObjectCommit& c )
	{
		Result r;
		return qHash( c.id( r ) );
	}

}

// Should we keep this? If yes, we should provide them for all GitWrap-classes.
/**
 * @ingroup		GitWrap
 * @brief		Debug-Stream support of Git::ObjectCommit
 * @param[in]	debug	The Debug-Stream to output into
 * @param[in]	commit	The commit object to output
 * @return		The Debug-Stream
 */
inline QDebug operator<<( QDebug debug, const Git::ObjectCommit& commit )
{
	Git::Result r;
	return debug << "Commit(id=" << commit.id( r ) << ";author=" << commit.author( r ) << ")";
}

#endif
