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

#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <QStringList>

#include "GitWrap.h"

namespace Git
{

	class Submodule;
	class Reference;
	class Error;
	class DiffList;
	class Index;
	class Object;
	class ObjectCommit;
	class ObjectTag;
	class ObjectBlob;
	class ObjectTree;
	class ObjectId;
	class RevisionWalker;
	class Remote;

	namespace Internal
	{
		class RepositoryPrivate;
	}

	typedef QHash< QString, ObjectId > ResolvedRefs;

	/**
	 * @ingroup		GitWrap
	 * @brief		A git repository
	 *
	 */
	class GITWRAP_API Repository
	{
	public:
		explicit Repository( Internal::RepositoryPrivate* _d );
		Repository();
		Repository( const Repository& other );
		Repository& operator=( const Repository& other );

	public:
		~Repository();

	public:
		static Repository create( const QString& path,
								  bool bare,
								  Result& result = GitWrap::lastResult() );

		static QString discover( const QString& startPath,
								 bool acrossFs = false,
								 const QStringList& ceilingDirs = QStringList(),
								 Result& result = GitWrap::lastResult() );

		static Repository open( const QString& path,
								Result& result = GitWrap::lastResult() );

		bool isValid() const;
		bool isBare() const;

		QString basePath() const;
		QString gitPath() const;

		QStringList allReferences( Result& result = GitWrap::lastResult() );
		QStringList allBranches( Result& result = GitWrap::lastResult() );
		QString currentBranch( Result& result = GitWrap::lastResult() );
		QStringList branches( bool local, bool remote, Result& result = GitWrap::lastResult() );
		QStringList allTags( Result& result = GitWrap::lastResult() );

		ResolvedRefs allResolvedRefs( Result& result = GitWrap::lastResult() );

		bool renameBranch( const QString& oldName, const QString& newName, bool force = false,
						   Result& result = GitWrap::lastResult() );

		Index index( Result& result = GitWrap::lastResult() );

		StatusHash statusHash( Result& result = GitWrap::lastResult() );

		Reference HEAD( Result& result = GitWrap::lastResult() );

		Object lookup( const ObjectId& id, ObjectType ot = otAny,
					   Result& result = GitWrap::lastResult() );

		ObjectCommit lookupCommit( const ObjectId& id, Result& result = GitWrap::lastResult() );
		ObjectTree lookupTree( const ObjectId& id, Result& result = GitWrap::lastResult() );
		ObjectBlob lookupBlob( const ObjectId& id, Result& result = GitWrap::lastResult() );
		ObjectTag lookupTag( const ObjectId& id, Result& result = GitWrap::lastResult() );

		bool shouldIgnore( const QString& filePath, Result& result = GitWrap::lastResult() ) const;

		RevisionWalker newWalker( Result& result = GitWrap::lastResult() );

		QStringList allRemotes( Result& result = GitWrap::lastResult() ) const;
		Remote remote( const QString& remoteName, Result& result = GitWrap::lastResult() ) const;
		Remote createRemote( const QString& remoteName, const QString& url,
							 const QString& fetchSpec, Result& result = GitWrap::lastResult() );

		DiffList diffCommitToCommit( ObjectCommit oldCommit, ObjectCommit newCommit,
									 Result& result = GitWrap::lastResult() );

		DiffList diffTreeToTree( ObjectTree oldTree, ObjectTree newTree,
								 Result& result = GitWrap::lastResult());

		DiffList diffIndexToTree( ObjectTree oldTree, Result& result = GitWrap::lastResult() );

		DiffList diffTreeToWorkingDir( ObjectTree oldTree, Result& result = GitWrap::lastResult() );
		DiffList diffIndexToWorkingDir( Result& result = GitWrap::lastResult() );

		QList< Submodule > submodules( Result& result = GitWrap::lastResult() );
		Submodule submodule( const QString& name, Result& result = GitWrap::lastResult() );

	private:
		Internal::GitPtr< Internal::RepositoryPrivate > d;
	};

}

#endif
