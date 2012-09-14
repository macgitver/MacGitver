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

#include "GitWrapPrivate.h"
#include "Error.h"
#include "Index.h"
#include "IndexPrivate.h"
#include "Remote.h"
#include "RemotePrivate.h"
#include "Repository.h"
#include "RepositoryPrivate.h"
#include "Reference.h"
#include "ReferencePrivate.h"
#include "DiffList.h"
#include "DiffListPrivate.h"
#include "Object.h"
#include "ObjectPrivate.h"
#include "ObjectTag.h"
#include "ObjectTree.h"
#include "ObjectBlob.h"
#include "ObjectCommit.h"
#include "RevisionWalker.h"
#include "RevisionWalkerPrivate.h"
#include "Submodule.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	RepositoryPrivate::RepositoryPrivate( git_repository* repo )
		: mRepo( repo )
		, mIndex( NULL )
	{
	}

	RepositoryPrivate::~RepositoryPrivate()
	{
		Q_ASSERT( mRepo );
		Q_ASSERT( !mIndex );

		git_repository_free( mRepo );
	}

	bool RepositoryPrivate::handleErrors( int rc ) const
	{
		if( rc < 0 )
		{
			const git_error* giterror = giterr_last();
			Error err( QString::fromLocal8Bit( giterror->message ) );
			giterr_clear();

			mErrorListMtx.lock();
			mErrors.append( err );
			mErrorListMtx.unlock();

			qDebug( "git2-Error: %s", qPrintable( err.text() ) );

			return false;
		}
		return true;
	}

	END_INTERNAL_IMPL()

	/**
	 * @internal
	 * @brief		Create a Repository object
	 * @param[in]	_d	Pointer to private data.
	 */
	Repository::Repository( Internal::RepositoryPrivate* _d )
		: d( _d )
	{
	}

	/**
	 * @brief		Copy Constructor
	 *
	 * Creates a Repository object that represents the same repository as @a o. If @a o is an
	 * invalid Repository object, this will become an invalid one too.
	 *
	 * @param[in]	o	An existing Repository object
	 *
	 */
	Repository::Repository( const Repository& o )
		: d( o.d )
	{
	}

	/**
	 * @brief		Create an invalid Repository object
	 */
	Repository::Repository()
	{
	}

	/**
	 * @brief		Destructor
	 */
	Repository::~Repository()
	{
	}

	/**
	 * @brief		Assignment operator
	 *
	 * @param[in]	o	An existing Repository object
	 *
	 * @return	A reference to this repository object.
	 */
	Repository& Repository::operator=( const Repository& o )
	{
		d = o.d;
		return * this;
	}

	/**
	 * @brief		Check validty of this repository
	 *
	 * @return		`true`, if this repository is valid, `false` otherwise.
	 */
	bool Repository::isValid() const
	{
		return d;
	}

	/**
	 * @brief		Create a new repository
	 *
	 * A new git repository will be created in @a path. The path pointed to by @a path must either
	 * be empty or not exist at all.
	 *
	 * Capabilities of the current operating system and the file system at @a path will be checked
	 * and the repository's config will be setup accordingly.
	 *
	 * @param[in]		path	The path where the new repository will be created.
	 * @param[in]		bare	If `true`, a bare repository will be created. If `false` a working
	 *							tree will be setup.
	 * @param[in,out]	result	A Result object; see @ref GitWrapErrorHandling
	 *
	 * @return	If successful, a `Repository` object for the newly created repostiory will be
	 *			returned. Otherwise an invalid `Repository` object will be returned and the Result
	 *			object is filled with the error.
	 *
	 * @see	Repository::open()
	 */
	Repository Repository::create( const QString& path,
								   bool bare,
								   Result& result )
	{
		if( !result )
		{
			return Repository();
		}

		git_repository* repo = NULL;
		result = git_repository_init( &repo, path.toUtf8().constData(), bare );

		if( !result )
		{
			return Repository();
		}

		return Repository( new Internal::RepositoryPrivate( repo ) );
	}

	/**
	 * @brief Lookup a git repository by walking parent directories starting from startPath
	 *
	 * The lookup ends when the first repository is found or when reaching one of the @a ceilingDirs
	 * directories.
	 *
	 * The method will automatically detect if the repository is bare (if there is a repository).
	 *
	 * @param[in] startPath
	 * The base path where the lookup starts.
	 *
	 * @param[in] acrossFs
	 * If `true`, then the lookup will not stop when a filesystem change is detected
	 * while exploring parent directories.
	 *
	 * @param[in] ceilingDirs
	 * A list of absolute paths (not symbolic links). The lookup will stop when one of these
	 * paths is reached and no repository was found.
	 *
	 * @param[in,out] result
	 * A result object; see @ref GitWrapErrorHandling
	 *
	 * @return the path of the found repository or an empty QString
	 *
	 * @see	Repository::open(), Repository::create()
	 */
	QString Repository::discover( const QString& startPath,
								  bool acrossFs,
								  const QStringList& ceilingDirs,
								  Result& result )
	{
		if( !result )
		{
			return QString();
		}

		QByteArray repoPath(GIT_PATH_MAX, Qt::Uninitialized);
		QByteArray joinedCeilingDirs = ceilingDirs.join(QChar::fromLatin1(GIT_PATH_LIST_SEPARATOR)).toUtf8();

		result = git_repository_discover( repoPath.data(), repoPath.length(),
										  startPath.toUtf8().constData(), acrossFs,
										  joinedCeilingDirs.constData() );

		return result ? QString::fromUtf8(repoPath.constData()) : QString();
	}

	/**
	 * @brief		Open an existing repository
	 *
	 * Opens the repository at @a path. The repository may be bare or have a working tree.
	 *
	 * This method will not try to discover a repository, if there is no repository found at
	 * @a path.
	 *
	 * @param[in]		path	The path of the repository to open.
	 * @param[in,out]	result	A result object; see @ref GitWrapErrorHandling
	 *
	 * @return	If successful, a `Repository` object for the opened repostiory will be returned.
	 *			Otherwise an invalid `Repository` object will be returned and the Result object
	 *			is filled with the error.
	 *
	 * @sa	Repository::discover(), Repository::create()
	 */
	Repository Repository::open( const QString& path,
								 Result& result )
	{
		if( !result )
		{
			return Repository();
		}
		git_repository* repo = NULL;


		result = git_repository_open( &repo, path.toUtf8().constData() );
		if( !result )
		{
			return Repository();
		}

		return Repository( new Internal::RepositoryPrivate( repo ) );
	}

	/**
	 * @brief		Check if the repository is bare
	 *
	 * @return		`true`, if the repository is bare and `false` if not.
	 *
	 * This method will return `true` if this Repository object is invalid. It will also set the
	 * per-thread Result to "Invalid object".
	 */
	bool Repository::isBare() const
	{
		if( d )
		{
			return git_repository_is_bare( d->mRepo );
		}
		else
		{
			GitWrap::lastResult().setInvalidObject();
			return true;
		}
	}

	/**
	 * @brief			Access the repository's index object
	 *
	 * @param[in,out]	result	A result object; see @ref GitWrapErrorHandling
	 *
	 * @return			The repository's index object or an invalid Index object, if either the
	 *					repository isBare() or an invalid repository object.
	 *
	 * If this repository object is invalid, the @a result object will be set to
	 * "Invalid object".
	 */
	Index Repository::index( Result& result )
	{
		if( !result )
		{
			return Index();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Index();
		}

		if( isBare() )
		{
			return Index();
		}

		if( !d->mIndex )
		{
			git_index* index = NULL;

			result = git_repository_index( &index, d->mRepo );

			if( !result )
			{
				return Index();
			}

			d->mIndex = new Internal::IndexPrivate( d, index );
		}

		return Index( d->mIndex );
	}

	/**
	 * @brief			List all references
	 *
	 * @param[in,out]	result	A Result object; see @ref GitWrapErrorHandling
	 *
	 * @return	A QStringList with all references of this repository.
	 *
	 */
	QStringList Repository::allReferences( Result& result )
	{
		if( !result )
		{
			return QStringList();
		}

		if( !d )
		{
			result.setInvalidObject();
			return QStringList();
		}

		git_strarray arr;
		result = git_reference_list( &arr, d->mRepo, GIT_REF_LISTALL );
		if( !result )
		{
			return QStringList();
		}

		return Internal::slFromStrArray( &arr );
	}

	namespace Internal
	{

		struct cb_enum_resolvedrefs_data
		{
			Result*			result;
			ResolvedRefs	refs;
			git_repository*	repo;
		};

		static int cb_enum_resolvedrefs( const char* refName, void* payload )
		{
			cb_enum_resolvedrefs_data* d = (cb_enum_resolvedrefs_data*) payload;

			git_oid oid;

			int rc = git_reference_name_to_oid( &oid, d->repo, refName );

			d->result->setError( rc );
			if( rc < 0 )
			{
				return -1;
			}

			QString name = QString::fromUtf8( refName );
			ObjectId obj = ObjectId::fromRaw( oid.id );

			d->refs.insert( name, obj );

			return 0;
		}

	}

	ResolvedRefs Repository::allResolvedRefs( Result& result )
	{
		if( !result )
		{
			return ResolvedRefs();
		}

		if( !d )
		{
			result.setInvalidObject();
			return ResolvedRefs();
		}

		Internal::cb_enum_resolvedrefs_data data;
		data.repo = d->mRepo;
		data.result = &result;

		Result tmp( git_reference_foreach( d->mRepo, GIT_REF_LISTALL,
										   &Internal::cb_enum_resolvedrefs, &data ) );

		if( tmp.errorCode() == GIT_EUSER )
		{
			// correct error is already in result.
			return ResolvedRefs();
		}

		if( !tmp )
		{
			result = tmp;
			return ResolvedRefs();
		}

		return data.refs;
	}

	QStringList Repository::allBranches()
	{
		return branches( true, true );
	}

	QString Repository::currentBranch()
	{
		Git::Reference refHEAD = HEAD();

		if( refHEAD.isValid() )
		{
			if( refHEAD.name() == QLatin1String( "HEAD" ) )
			{
				return QString();
			}
			return refHEAD.name().mid( 11 );
		}

		return QString();
	}

	static int listBranches( const char* branchName, git_branch_t brachType, void* payload )
	{
		QStringList* sl = (QStringList*) payload;
		sl->append( QString::fromUtf8( branchName ) );
		return 0;
	}

	QStringList Repository::branches( bool local, bool remote )
	{
		Q_ASSERT( d );

		QStringList sl;
		int rc = git_branch_foreach( d->mRepo,
									 ( local ? GIT_BRANCH_LOCAL : 0 ) |
									 ( remote ? GIT_BRANCH_REMOTE : 0 ),
									 &listBranches,
									 (void*) &sl );
		if( !d->handleErrors( rc ) )
		{
			return QStringList();
		}

		return sl;
	}

	bool Repository::renameBranch( const QString& oldName, const QString& newName, bool force )
	{
		if( !d )
		{
			return false;
		}

		git_reference* ref = NULL;

		int rc = git_branch_lookup( &ref, d->mRepo, oldName.toUtf8().constData(), GIT_BRANCH_LOCAL );
		if( rc == GITERR_REFERENCE )
		{
			rc = git_branch_lookup( &ref, d->mRepo, oldName.toUtf8().constData(), GIT_BRANCH_REMOTE );
		}
		if( !d->handleErrors( rc ) )
		{
			return false;
		}

		rc = git_branch_move( ref, newName.toUtf8().constData(), force );
		git_reference_free( ref );

		return d->handleErrors( rc );
	}

	QStringList Repository::allTags()
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_tag_list( &arr, d->mRepo );
		if( !d->handleErrors( rc ) )
		{
			return QStringList();
		}

		return Internal::slFromStrArray( &arr );
	}

	namespace Internal
	{

		static int statusHashCB( const char* fn, unsigned int status, void* rawSH )
		{
			#if 0
			qDebug( "%s - %s", qPrintable( QString::number( status, 2 ) ), fn );
			#endif

			StatusHash* sh = (StatusHash*) rawSH;
			sh->insert( QString::fromUtf8( fn ), FileStati( status ) );

			return GIT_OK;
		}

	}

	StatusHash Repository::statusHash( Result& result )
	{
		if( !result )
		{
			return StatusHash();
		}

		if( !d )
		{
			result.setInvalidObject();
			return StatusHash();
		}

		StatusHash sh;

		git_status_options opt;
		memset( &opt, 0, sizeof( opt ) );

		opt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED
				  | GIT_STATUS_OPT_INCLUDE_IGNORED
				  | GIT_STATUS_OPT_INCLUDE_UNMODIFIED
				  | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;

		result = git_status_foreach_ext( d->mRepo, &opt, &Internal::statusHashCB, (void*) &sh );
		if( !result )
		{
			return StatusHash();
		}

		return sh;
	}

	QString Repository::basePath() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return QString();
		}

		return QString::fromUtf8( git_repository_workdir( d->mRepo ) );
	}

	QString Repository::gitPath() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return QString();
		}

		return QString::fromUtf8( git_repository_path( d->mRepo ) );
	}

	Reference Repository::HEAD( Result& result )
	{
		if( !result )
		{
			return Reference();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Reference();
		}

		git_reference* refHead = NULL;

		result = git_repository_head( &refHead, d->mRepo );
		if( !result )
		{
			return Reference();
		}

		return new Internal::ReferencePrivate( d, refHead );
	}

	Object Repository::lookup( const ObjectId& id, ObjectType ot, Result& result )
	{
		if( !result )
		{
			return Object();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Object();
		}

		git_object* obj = NULL;
		git_otype gitObjType;

		switch( ot )
		{
		case otAny:		gitObjType = GIT_OBJ_ANY;		break;
		case otCommit:	gitObjType = GIT_OBJ_COMMIT;	break;
		case otTree:	gitObjType = GIT_OBJ_TREE;		break;
		case otTag:		gitObjType = GIT_OBJ_TAG;		break;
		case otBlob:	gitObjType = GIT_OBJ_BLOB;		break;
		default:		Q_ASSERT( false ); return Object();
		}

		result = git_object_lookup( &obj, d->mRepo, (git_oid*) id.raw(), gitObjType );
		if( !result )
		{
			return Object();
		}

		return new Internal::ObjectPrivate( d, obj );
	}

	ObjectCommit Repository::lookupCommit( const ObjectId& id, Result& result )
	{
		return lookup( id, otCommit, result ).asCommit();
	}

	ObjectTree Repository::lookupTree( const ObjectId& id, Result& result )
	{
		return lookup( id, otTree, result ).asTree();
	}

	ObjectBlob Repository::lookupBlob( const ObjectId& id, Result& result )
	{
		return lookup( id, otBlob, result ).asBlob();
	}

	ObjectTag Repository::lookupTag( const ObjectId& id, Result& result )
	{
		return lookup( id, otTag, result ).asTag();
	}

	RevisionWalker Repository::newWalker( Result& result )
	{
		if( !result )
		{
			return RevisionWalker();
		}

		if( !d )
		{
			result.setInvalidObject();
			return RevisionWalker();
		}

		git_revwalk* walker = NULL;

		result = git_revwalk_new( &walker, d->mRepo );
		if( !result )
		{
			return RevisionWalker();
		}

		return new Internal::RevisionWalkerPrivate( d, walker );
	}

	bool Repository::shouldIgnore( const QString& filePath, Result& result ) const
	{
		if( !result )
		{
			return false;
		}

		if( !d )
		{
			result.setInvalidObject();
			return false;
		}

		int ignore = 0;

		result = git_status_should_ignore( &ignore, d->mRepo, filePath.toUtf8().constData() );
		if( !result )
		{
			return false;
		}

		return ignore;
	}

	QStringList Repository::allRemotes( Result& result ) const
	{
		if( !result )
		{
			return QStringList();
		}

		if( !d )
		{
			result.setInvalidObject();
			return QStringList();
		}

		git_strarray arr;
		result = git_remote_list( &arr, d->mRepo );
		if( !result )
		{
			return QStringList();
		}

		return Internal::slFromStrArray( &arr );
	}

	Remote Repository::remote( const QString& remoteName, Result& result ) const
	{
		if( !result )
		{
			return Remote();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Remote();
		}

		git_remote* remote = NULL;
		result = git_remote_load( &remote, d->mRepo, remoteName.toUtf8().constData() );

		if( !result )
		{
			return Remote();
		}

		return new Internal::RemotePrivate( const_cast< Internal::RepositoryPrivate* >( *d ), remote );
	}

	Remote Repository::createRemote( const QString& remoteName, const QString& url,
									 const QString& fetchSpec, Result& result )
	{
		if( !result )
		{
			return Remote();
		}

		if( !d )
		{
			result.setInvalidObject();
			return Remote();
		}

		git_remote* remote = NULL;
		result = git_remote_new( &remote, d->mRepo, remoteName.toUtf8().constData(),
								 url.toUtf8().constData(), fetchSpec.toUtf8().constData() );
		if( !result )
		{
			return Remote();
		}

		return new Internal::RemotePrivate( *d, remote );
	}

	DiffList Repository::diffCommitToCommit( ObjectCommit oldCommit, ObjectCommit newCommit,
											 Result& result )
	{
		return diffTreeToTree( oldCommit.tree(), newCommit.tree(), result );
	}

	DiffList Repository::diffTreeToTree( ObjectTree oldTree, ObjectTree newTree,
										 Result& result  )
	{
		return oldTree.diffToTree( newTree, result );
	}

	DiffList Repository::diffIndexToTree( ObjectTree oldTree, Result& result )
	{
		return oldTree.diffToIndex( result );
	}

	DiffList Repository::diffTreeToWorkingDir( ObjectTree oldTree, Result& result )
	{
		return oldTree.diffToWorkingDir( result );
	}

	DiffList Repository::diffIndexToWorkingDir( Result& result )
	{
		if( !result )
		{
			return DiffList();
		}

		if( !d )
		{
			result.setInvalidObject();
			return DiffList();
		}

		git_diff_list* diffList = NULL;
		result = git_diff_workdir_to_index( d->mRepo, NULL, &diffList );
		if( !result )
		{
			return DiffList();
		}

		return DiffList( new Internal::DiffListPrivate( d, diffList ) );
	}

	struct cb_enum_submodules_t
	{
		QList< Submodule >* subs;
		Internal::RepositoryPrivate* repo;
	};

	static int cb_enum_submodules( git_submodule* sm, const char* name, void* payload )
	{
		cb_enum_submodules_t* d = static_cast< cb_enum_submodules_t* >( payload );
		Q_ASSERT( d && d->subs && name );

		d->subs->append( Submodule( d->repo, QString::fromUtf8( name ) ) );
		return 0;
	}

	QList< Submodule > Repository::submodules()
	{
		QList< Submodule > result;

		cb_enum_submodules_t data = { &result, d };

		int rc = git_submodule_foreach( d->mRepo, &cb_enum_submodules, &data );
		if( !d->handleErrors( rc ) )
		{
			return QList< Submodule >();
		}

		return result;
	}

	Submodule Repository::submodule( const QString& name )
	{
		return Submodule( d, name );
	}

}
