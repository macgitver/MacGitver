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

	Repository::~Repository()
	{
	}

	Repository& Repository::operator=( const Repository& o )
	{
		d = o.d;
		return * this;
	}

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
	 * A Result object; see @ref GitWrapErrorHandling
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
	 * @param[in,out]	result	A Result object; see @ref GitWrapErrorHandling
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

	bool Repository::isBare() const
	{
		Q_ASSERT( d );
		return git_repository_is_bare( d->mRepo );
	}

	Index Repository::index()
	{
		Q_ASSERT( d );

		if( isBare() )
		{
			return Index();
		}

		if( !d->mIndex )
		{
			git_index* index = NULL;

			int rc = git_repository_index( &index, d->mRepo );

			if( !d->handleErrors( rc ) )
			{
				return Index();
			}

			d->mIndex = new Internal::IndexPrivate( d, index );
		}

		return Index( d->mIndex );
	}

	QStringList slFromStrArray( git_strarray* arry )
	{
		QStringList sl;

		for( unsigned int i = 0; i < arry->count; i++ )
		{
			sl << QString::fromUtf8( arry->strings[ i ] );
		}

		git_strarray_free( arry );
		return sl;
	}

	QStringList Repository::allReferences()
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_reference_list( &arr, d->mRepo, GIT_REF_LISTALL );
		if( !d->handleErrors( rc ) )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}

	struct cb_enum_resolvedrefs_data
	{
		ResolvedRefs	refs;
		git_repository*	repo;
	};

	static int cb_enum_resolvedrefs( const char* refName, void* payload )
	{
		cb_enum_resolvedrefs_data* d = (cb_enum_resolvedrefs_data*) payload;

		git_oid oid;
		if( git_reference_name_to_oid( &oid, d->repo, refName ) < 0 )
		{
			return -1;
		}

		QString name = QString::fromUtf8( refName );
		ObjectId obj = ObjectId::fromRaw( oid.id );

		d->refs.insert( name, obj );

		return 0;
	}

	ResolvedRefs Repository::allResolvedRefs()
	{
		Q_ASSERT( d );

		cb_enum_resolvedrefs_data data;
		data.repo = d->mRepo;
		git_reference_foreach( d->mRepo, GIT_REF_LISTALL, &cb_enum_resolvedrefs, &data );

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

		return slFromStrArray( &arr );
	}

	static int statusHashCB( const char* fn, unsigned int status, void* rawSH )
	{
		#if 0
		qDebug( "%s - %s", qPrintable( QString::number( status, 2 ) ), fn );
		#endif
		StatusHash* sh = (StatusHash*) rawSH;
		sh->insert( QString::fromUtf8( fn ), FileStati( status ) );
		return GIT_OK;
	}

	StatusHash Repository::statusHash()
	{
		StatusHash sh;

		if( d )
		{
			git_status_options opt;
			memset( &opt, 0, sizeof( opt ) );
			opt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED
					  | GIT_STATUS_OPT_INCLUDE_IGNORED
					  | GIT_STATUS_OPT_INCLUDE_UNMODIFIED
					  | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
			int rc = git_status_foreach_ext( d->mRepo, &opt, &statusHashCB, (void*) &sh );
			if( !d->handleErrors( rc ) )
			{
				return StatusHash();
			}
		}

		return sh;
	}

	QString Repository::basePath() const
	{
		if( !d )
			return QString();
		return QString::fromUtf8( git_repository_workdir( d->mRepo ) );
	}

	QString Repository::gitPath() const
	{
		if( !d )
			return QString();
		return QString::fromUtf8( git_repository_path( d->mRepo ) );
	}

	Reference Repository::HEAD()
	{
		Q_ASSERT( d );
		Reference ref;

		if( d )
		{
			git_reference* refHead = NULL;
			int rc = git_repository_head( &refHead, d->mRepo );
			if( !d->handleErrors( rc ) )
			{
				return Reference();
			}

			ref = new Internal::ReferencePrivate( d, refHead );
		}

		return ref;
	}

	Object Repository::lookup( const ObjectId& id, ObjectType ot )
	{
		Q_ASSERT( d );

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

		int rc = git_object_lookup( &obj, d->mRepo, (git_oid*) id.raw(), gitObjType );
		if( !d->handleErrors( rc ) )
		{
			return Object();
		}

		return new Internal::ObjectPrivate( d, obj );
	}

	ObjectCommit Repository::lookupCommit( const ObjectId& id )
	{
		return lookup( id, otCommit ).asCommit();
	}

	ObjectTree Repository::lookupTree( const ObjectId& id )
	{
		return lookup( id, otTree ).asTree();
	}

	ObjectBlob Repository::lookupBlob( const ObjectId& id )
	{
		return lookup( id, otBlob ).asBlob();
	}

	ObjectTag Repository::lookupTag( const ObjectId& id )
	{
		return lookup( id, otTag ).asTag();
	}

	RevisionWalker Repository::newWalker()
	{
		Q_ASSERT( d );
		if( d )
		{
			git_revwalk* walker = NULL;

			int rc = git_revwalk_new( &walker, d->mRepo );
			if( !d->handleErrors( rc ) )
			{
				return RevisionWalker();
			}

			return new Internal::RevisionWalkerPrivate( d, walker );
		}

		return RevisionWalker();
	}

	bool Repository::shouldIgnore( const QString& filePath ) const
	{
		int ignore = 0;
		if( d )
		{
			int rc = git_status_should_ignore( &ignore, d->mRepo, filePath.toUtf8().constData() );
			if( !d->handleErrors( rc ) )
			{
				return false;
			}
		}
		return ignore;
	}

	QStringList Repository::allRemotes() const
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_remote_list( &arr, d->mRepo );
		if( !d->handleErrors( rc ) )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}

	Remote Repository::remote( const QString& remoteName ) const
	{
		Q_ASSERT( d );

		git_remote* remote = NULL;
		int rc = git_remote_load( &remote, d->mRepo, remoteName.toUtf8().constData() );
		if( !d->handleErrors( rc ) )
		{
			return Remote();
		}

		return new Internal::RemotePrivate( const_cast< Internal::RepositoryPrivate* >( *d ), remote );
	}

	Remote Repository::createRemote( const QString& remoteName, const QString& url,
									 const QString& fetchSpec )
	{
		Q_ASSERT( d );

		git_remote* remote = NULL;
		int rc = git_remote_new( &remote, d->mRepo, remoteName.toUtf8().constData(),
								 url.toUtf8().constData(), fetchSpec.toUtf8().constData() );
		if( !d->handleErrors( rc ) )
		{
			return Remote();
		}

		return new Internal::RemotePrivate( *d, remote );
	}

	DiffList Repository::diffCommitToCommit( ObjectCommit oldCommit, ObjectCommit newCommit )
	{
		return diffTreeToTree( oldCommit.tree(), newCommit.tree() );
	}

	DiffList Repository::diffTreeToTree( ObjectTree oldTree, ObjectTree newTree )
	{
		return oldTree.diffToTree( newTree );
	}

	DiffList Repository::diffIndexToTree( ObjectTree oldTree )
	{
		return oldTree.diffToIndex();
	}

	DiffList Repository::diffTreeToWorkingDir( ObjectTree oldTree )
	{
		return oldTree.diffToWorkingDir();
	}

	DiffList Repository::diffIndexToWorkingDir()
	{
		if( !d )
		{
			return DiffList();
		}

		git_diff_list* diffList = NULL;
		int rc = git_diff_workdir_to_index( d->mRepo, NULL, &diffList );
		if( !d->handleErrors( rc ) )
		{
			return DiffList();
		}

		return DiffList( new Internal::DiffListPrivate( d, diffList ) );
	}


	QList< Error > Repository::recentErrors()
	{
		QList< Error > detached;

		d->mErrorListMtx.lock();
		detached = d->mErrors;
		d->mErrors.clear();
		d->mErrorListMtx.unlock();

		return detached;
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
