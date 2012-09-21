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
			return isEqual( commit );
		}

		bool operator!=( const Git::ObjectCommit& commit ) const
		{
			return !isEqual( commit );
		}

	public:
		ObjectTree tree( Result& result = GitWrap::lastResult() );
		ObjectId treeId( Result& result = GitWrap::lastResult() );

		ObjectIdList parentCommitIds( Result& result = GitWrap::lastResult() ) const;
		QList< ObjectCommit > parentCommits( Result& result = GitWrap::lastResult() ) const;
		ObjectCommit parentCommit( unsigned int index, Result& result = GitWrap::lastResult() ) const;
		ObjectId parentCommitId( unsigned int index, Result& result = GitWrap::lastResult() ) const;
		unsigned int numParentCommits() const;

		bool isParentOf( const Git::ObjectCommit& child, Result& result = GitWrap::lastResult() ) const;
		bool isChildOf( const Git::ObjectCommit& parent, Result& result = GitWrap::lastResult() ) const;
		bool isEqual( const Git::ObjectCommit& commit, Result& result = GitWrap::lastResult() ) const;

		Signature author( Result& result = GitWrap::lastResult() ) const;
		Signature committer( Result& result = GitWrap::lastResult() ) const;

		QString message( Result& result = GitWrap::lastResult() ) const;
		QString shortMessage( Result& result = GitWrap::lastResult() ) const;

		Reference createBranch( const QString& name, bool force,
								Result& result = GitWrap::lastResult() );

		DiffList diffFromParent( unsigned int index, Result& result = GitWrap::lastResult() );
		DiffList diffFromAllParents( Result& result = GitWrap::lastResult() );
	};

	/**
	 * @ingroup		GitWrap
	 * @brief		qHash() for Git::ObjectCommit
	 */
	inline uint qHash( const ObjectCommit& c )
	{
		return qHash( c.id() );
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
	return debug << "Commit(id=" << commit.id() << ";author=" << commit.author() << ")";
}

#endif
