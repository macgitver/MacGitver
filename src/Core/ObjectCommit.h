
#ifndef GIT_OBJECT_COMMIT_H
#define GIT_OBJECT_COMMIT_H

#include <QDebug>
#include <QList>

#include "Core/Git.h"
#include "Core/ObjectId.h"
#include "Core/Object.h"
#include "Core/Signature.h"

namespace Git
{

	class ObjectTree;

	class ObjectCommit : public Object
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
