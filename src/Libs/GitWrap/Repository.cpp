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

#include "git2/branch.h"

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

	QList< QByteArray > byteArrayListFromStrArray( git_strarray* arry )
	{
		QList< QByteArray > list;

		for( unsigned int i = 0; i < arry->count; i++ )
		{
			list << QByteArray( arry->strings[ i ] );
		}

		git_strarray_free( arry );
		return list;
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

	QStringList Repository::allBranches()
	{
		return branches( true, true );
	}

	QStringList Repository::branches( bool local, bool remote )
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_branch_list( &arr, d->mRepo,
								  ( local ? GIT_BRANCH_LOCAL : 0 ) |
								  ( remote ? GIT_BRANCH_REMOTE : 0 ) );
		if( !d->handleErrors( rc ) )
		{
			return QStringList();
		}

		return slFromStrArray( &arr );
	}

	bool Repository::deleteBranch( const QString& name, bool local )
	{
		if( !d )
		{
			return false;
		}

		int rc = git_branch_delete( d->mRepo, name.toUtf8().constData(),
									local ? GIT_BRANCH_LOCAL : GIT_BRANCH_REMOTE );

		return d->handleErrors( rc );
	}

	bool Repository::renameBranch( const QString& oldName, const QString& newName, bool force )
	{
		if( !d )
		{
			return false;
		}

		int rc = git_branch_move( d->mRepo,
								  oldName.toUtf8().constData(),
								  newName.toUtf8().constData(),
								  force );

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
				qPrintable( name ) );
		return GIT_OK;
	}

	void Repository::test()
	{
		git_status_foreach( d->mRepo, &status_callback, 0 );
	}

	static int statusHashCB( const char* fn, unsigned int status, void* rawSH )
	{
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
		return shouldIgnore( filePath.toLocal8Bit() );
	}

	bool Repository::shouldIgnore( const QByteArray& filePath ) const
	{
		int ignore = 0;
		if( d )
		{
			int rc = git_status_should_ignore( &ignore, d->mRepo, filePath.data() );
			if( !d->handleErrors( rc ) )
			{
				return false;
			}
		}
		return ignore;
	}

	QList< QByteArray > Repository::allRemotes() const
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_remote_list( &arr, d->mRepo );
		if( !d->handleErrors( rc ) )
		{
			return QList< QByteArray >();
		}

		return byteArrayListFromStrArray( &arr );
	}

	Remote Repository::remote( const QByteArray& remoteName ) const
	{
		Q_ASSERT( d );

		git_remote* remote = NULL;
		int rc = git_remote_load( &remote, d->mRepo, remoteName.constData() );
		if( !d->handleErrors( rc ) )
		{
			return Remote();
		}

		return new RemotePrivate( const_cast< RepositoryPrivate* >( *d ), remote );
	}

	Remote Repository::createRemote( const QByteArray& remoteName, const QByteArray& url,
									 const QByteArray& fetchSpec )
	{
		Q_ASSERT( d );

		git_remote* remote = NULL;
		int rc = git_remote_new( &remote, d->mRepo, remoteName.constData(),
								 url.constData(), fetchSpec.constData() );
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

}
