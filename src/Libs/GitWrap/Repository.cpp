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

#include "Git_p.h"
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

#include <QFile>

namespace Git
{

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

	void RepositoryPrivate::ref()
	{
		mRefCounter.ref();
	}

	void RepositoryPrivate::deref()
	{
		if( !mRefCounter.deref() )
		{
			delete this;
		}
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

	Repository::Repository( RepositoryPrivate* _d )
		: d( _d )
	{
	}

	Repository::Repository( const Repository& o )
		: d( o.d )
	{
	}

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

	Repository Repository::create( const QString& path, bool bare )
	{
		git_repository* repo = NULL;

		int rc = git_repository_init( &repo, path.toLatin1().constData(), bare );
		if( rc < GIT_OK )
		{
			return Repository();
		}

        return Repository( new RepositoryPrivate( repo ) );
    }

    QString Repository::discover(const QString& startPath, bool acrossFs, const QStringList& ceilingDirs)
    {
        QByteArray repoPath(GIT_PATH_MAX, Qt::Uninitialized);
        QByteArray joinedCeilingDirs = QFile::encodeName( ceilingDirs.join(QChar::fromLatin1(GIT_PATH_LIST_SEPARATOR)) );

        git_repository_discover( repoPath.data(), repoPath.length()
                                 , QFile::encodeName(startPath).constData()
                                 , acrossFs, joinedCeilingDirs.constData()
                                 );

        return QFile::decodeName(repoPath);
    }

	Repository Repository::open( const QString& path )
	{
		git_repository* repo = NULL;

		int rc = git_repository_open( &repo, path.toLatin1().constData() );
		if( rc < GIT_OK )
		{
			return Repository();
		}

		return Repository( new RepositoryPrivate( repo ) );
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

			d->mIndex = new IndexPrivate( d, index );
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


	int status_callback( const char* name, unsigned int status, void* )
	{
		qDebug( "%s - %s",
				qPrintable( QString::number( status, 2 ) ),
				name );
		return GIT_OK;
	}

	void Repository::test()
	{
		git_status_foreach( d->mRepo, &status_callback, 0 );
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

			ref = new ReferencePrivate( d, refHead );
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

		return new ObjectPrivate( d, obj );
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

			return new RevisionWalkerPrivate( d, walker );
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

		return new RemotePrivate( const_cast< RepositoryPrivate* >( *d ), remote );
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

		return new RemotePrivate( const_cast< RepositoryPrivate* >( *d ), remote );
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

		return DiffList( new DiffListPrivate( d, diffList ) );
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
		RepositoryPrivate* repo;
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
