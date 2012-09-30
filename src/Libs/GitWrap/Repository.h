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
								  Result& result GITWRAP_DEFAULT_TLSRESULT );

		static QString discover( const QString& startPath,
								 bool acrossFs /* = false */,
								 const QStringList& ceilingDirs /* = QStringList() */,
								 Result& result GITWRAP_DEFAULT_TLSRESULT );

		static Repository open( const QString& path,
								Result& result GITWRAP_DEFAULT_TLSRESULT );

		bool isValid() const;
		bool isBare() const;

		QString basePath() const;
		QString gitPath() const;

		QStringList allReferences( Result& result GITWRAP_DEFAULT_TLSRESULT );
		QStringList allBranches( Result& result GITWRAP_DEFAULT_TLSRESULT );
		QString currentBranch( Result& result GITWRAP_DEFAULT_TLSRESULT );
		QStringList branches( bool local, bool remote, Result& result GITWRAP_DEFAULT_TLSRESULT );
		QStringList allTags( Result& result GITWRAP_DEFAULT_TLSRESULT );

		ResolvedRefs allResolvedRefs( Result& result GITWRAP_DEFAULT_TLSRESULT );

		bool renameBranch( const QString& oldName, const QString& newName, bool force /* = false */,
						   Result& result GITWRAP_DEFAULT_TLSRESULT );

		Index index( Result& result GITWRAP_DEFAULT_TLSRESULT );

		StatusHash statusHash( Result& result GITWRAP_DEFAULT_TLSRESULT );

		Reference HEAD( Result& result GITWRAP_DEFAULT_TLSRESULT );
		Reference lookupRef( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectId resolveRef( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );

		Object lookup( const ObjectId& id, ObjectType ot /* = otAny */,
					   Result& result GITWRAP_DEFAULT_TLSRESULT );

		ObjectCommit lookupCommit( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectTree lookupTree( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectBlob lookupBlob( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectTag lookupTag( const ObjectId& id, Result& result GITWRAP_DEFAULT_TLSRESULT );

		Object lookup( const QString& refName, ObjectType ot /* = otAny */,
					   Result& result GITWRAP_DEFAULT_TLSRESULT );

		ObjectCommit lookupCommit( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectTree lookupTree( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectBlob lookupBlob( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );
		ObjectTag lookupTag( const QString& refName, Result& result GITWRAP_DEFAULT_TLSRESULT );

		bool shouldIgnore( const QString& filePath, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;

		RevisionWalker newWalker( Result& result GITWRAP_DEFAULT_TLSRESULT );

		QStringList allRemotes( Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		Remote remote( const QString& remoteName, Result& result GITWRAP_DEFAULT_TLSRESULT ) const;
		Remote createRemote( const QString& remoteName, const QString& url,
							 const QString& fetchSpec, Result& result GITWRAP_DEFAULT_TLSRESULT );

		DiffList diffCommitToCommit( ObjectCommit oldCommit, ObjectCommit newCommit,
									 Result& result GITWRAP_DEFAULT_TLSRESULT );

		DiffList diffTreeToTree( ObjectTree oldTree, ObjectTree newTree,
								 Result& result GITWRAP_DEFAULT_TLSRESULT);

		DiffList diffIndexToTree( ObjectTree oldTree, Result& result GITWRAP_DEFAULT_TLSRESULT );

		DiffList diffTreeToWorkingDir( ObjectTree oldTree, Result& result GITWRAP_DEFAULT_TLSRESULT );
		DiffList diffIndexToWorkingDir( Result& result GITWRAP_DEFAULT_TLSRESULT );

		QList< Submodule > submodules( Result& result GITWRAP_DEFAULT_TLSRESULT );
		Submodule submodule( const QString& name, Result& result GITWRAP_DEFAULT_TLSRESULT );

	private:
		Internal::GitPtr< Internal::RepositoryPrivate > d;
	};

}

Q_DECLARE_METATYPE( Git::Repository )

#endif
