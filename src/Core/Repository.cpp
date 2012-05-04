
#include "Core/Git_p.h"
#include "Core/Index.h"
#include "Core/Repository.h"

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

	void RepositoryPrivate::setError( int rc )
	{
		Q_UNUSED( rc );
		QString errText = QString::fromUtf8( git_lasterror() );
		qDebug( "%s", qPrintable( errText ) );
	//	MainWindow::self().addError( errText );
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
		if( rc < GIT_SUCCESS )
		{
		//	d->setError( rc );
			return Repository();
		}

		return Repository( new RepositoryPrivate( repo ) );
	}

	Repository Repository::open( const QString& path )
	{
		git_repository* repo = NULL;

		int rc = git_repository_open( &repo, path.toLatin1().constData() );

		if( rc < GIT_SUCCESS )
		{
		//	d->setError( rc );
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

			if( rc < GIT_SUCCESS )
			{
				d->setError( rc );
				return Index();
			}

			d->mIndex = new IndexPrivate( d, index );
		}

		return Index( d->mIndex );
	}

	QStringList Repository::allReferences()
	{
		Q_ASSERT( d );

		git_strarray arr;
		int rc = git_reference_listall( &arr, d->mRepo, GIT_REF_LISTALL );
		if( rc < GIT_SUCCESS )
		{
			return QStringList();
		}

		QStringList sl;
		for( unsigned int i = 0; i < arr.count; i++ )
			sl << QString::fromLatin1( arr.strings[ i ] );

		return sl;
	}


	int status_callback( const char* name, unsigned int status, void* )
	{
		qDebug( "%s - %s",
				qPrintable( QString::number( status, 2 ) ),
				qPrintable( name ) );
		return GIT_SUCCESS;
	}

	void Repository::test()
	{
		git_status_foreach( d->mRepo, &status_callback, 0 );
	}

}
