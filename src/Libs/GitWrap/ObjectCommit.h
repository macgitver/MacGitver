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

#include "Git.h"
#include "ObjectId.h"
#include "Object.h"
#include "Signature.h"

namespace Git
{

	class ObjectTree;

	class GITWRAP_API ObjectCommit : public Object
	{
	public:
		ObjectCommit();
		ObjectCommit( ObjectPrivate* _d );
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
		ObjectTree tree();
		ObjectId treeId();

		QList< ObjectId > parentCommitIds() const;
		QList< ObjectCommit > parentCommits() const;
		ObjectCommit parentCommit( unsigned int index ) const;
		unsigned int numParentCommits() const;

		bool isParentOf( const Git::ObjectCommit& child ) const;
		bool isChildOf( const Git::ObjectCommit& parent ) const;
		bool isEqual( const Git::ObjectCommit& commit ) const;

		Signature author() const;
		Signature committer() const;

		QString message() const;
		QString shortMessage() const;
	};

	inline uint qHash( const ObjectCommit& c )
	{
		return qHash( c.id() );
	}

}

inline QDebug operator<<( QDebug debug, const Git::ObjectCommit& commit )
{
	return debug << "Commit(id=" << commit.id() << ";author=" << commit.author() << ")";
}

#endif
