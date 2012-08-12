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

#include <QFileSystemWatcher>
#include <QStringBuilder>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <QTimer>

#define DBG_FSWATCHER 0

#include "FSWatcher.h"

enum FileType
{
	UnknownFile			= 0,

	DirectoryFile		= 1 << 0,
	RootDirectoryFile	= 1 << 1,

	WorkingTreeFile		= 1 << 2,
	ConfigFile			= 1 << 3,
	RefsFile			= 1 << 4,
	HeadFile			= 1 << 5,
	ModeFile			= 1 << 6,
	IndexFile			= 1 << 7,
	DescriptionFile		= 1 << 8,
	RefLogFile			= 1 << 9,
	RepoGitFile			= 1 << 10	// a .gitanything file inside WT
};

typedef QFlags< FileType > FileTypes;

struct FileInfo
{
public:
	FileInfo();
	~FileInfo();

	QString fullPath() const
	{
		if( type == RootDirectoryFile )
			return path;
		return path % QChar( L'/' ) % name;
	}

public:
	FileType					type;
	FileInfo*					parent;
	QString						path;
	QString						name;
	QDateTime					modDttm;
	QHash< QString, FileInfo* >	children;
};

class FSWatcherPrivate
{
public:
	FSWatcherPrivate( FSWatcher* owner );

public:
	FileInfo* mkFileInfoForPath( const QString& path );

	void seekLostAndFoundFiles( FileInfo* path );

	void foundFile( FileInfo* path, const QString& name );
	void lostFile( FileInfo* path, const QString& name );
	void foundDirectory( FileInfo* path, const QString& name );
	void lostDirectory( FileInfo* path );
	void modifiedFile( FileInfo* path );
	void enqueueFood( FileTypes types );
	void spoonFeed();

public:
	FSWatcher* const	mOwner;
	Git::Repository		mRepo;
	QFileSystemWatcher*	mWatcher;
	FileInfo*			mRoot;
	bool				mSpitQueued;
	FileTypes			mLastChange;
	FileTypes			mCurrentChange;
};

FSWatcherPrivate::FSWatcherPrivate( FSWatcher* owner )
	: mOwner( owner )
{
	mCurrentChange = 0;
	mLastChange = 0;
	mSpitQueued = false;
	mRoot = NULL;
	mWatcher = NULL;
}

FileInfo* FSWatcherPrivate::mkFileInfoForPath( const QString& path )
{
	FileInfo* fi = new FileInfo;

	QDir d( path );
	fi->type = RootDirectoryFile;
	fi->path = d.absolutePath();
	fi->name = QString();
	fi->parent = NULL;

	return fi;
}

void FSWatcherPrivate::seekLostAndFoundFiles( FileInfo* path )
{
	Q_ASSERT( path );

	QString basePath = path->fullPath();

	QHash< QString, FileInfo* > leftover = path->children;
	path->children.clear();

	foreach( QString sub, QDir( basePath ).entryList( QDir::NoDotAndDotDot | QDir::Dirs | QDir::Hidden ) )
	{
		if( leftover.contains( sub ) )
		{
			FileInfo* subfi = leftover.take( sub );
			if( subfi->type != DirectoryFile )
			{
				lostFile( path, sub );
				foundDirectory( path, sub );
			}
			else
			{
				// just reinsert it
				path->children.insert( sub, subfi );
			}
		}
		else
		{
			foundDirectory( path, sub );
		}
	}

	foreach( QFileInfo fisub, QDir( basePath ).entryInfoList( QDir::Files ) )
	{
		QString sub = fisub.fileName();

		if( leftover.contains( sub ) )
		{
			FileInfo* subfi = leftover.take( sub );
			if( subfi->type == DirectoryFile )
			{
				lostDirectory( subfi );
				foundFile( path, sub );
			}
			else
			{
				// just reinsert it
				path->children.insert( sub, subfi );

				if( subfi->modDttm != fisub.lastModified() )
				{
					subfi->modDttm = fisub.lastModified();
					modifiedFile( subfi );
				}
			}
		}
		else
		{
			foundFile( path, sub );
		}
	}

	foreach( QString sub, leftover.keys() )
	{
		if( leftover[ sub ]->type == DirectoryFile )
		{
			lostDirectory( leftover[ sub ] );
		}
		else
		{
			lostFile( path, sub );
			delete leftover[ sub ];
		}
	}
}

void FSWatcherPrivate::foundFile( FileInfo* path, const QString& name )
{
	Q_ASSERT( path );

	QFileInfo rawFi( path->fullPath() % QChar( L'/' ) % name );

	FileInfo* fi = new FileInfo();
	fi->path = path->fullPath();
	fi->name = name;
	fi->parent = path;
	fi->modDttm = rawFi.lastModified();

	QString gitPath = mRepo.gitPath();
	if( gitPath.endsWith( L'/' ) )
	{
		gitPath = gitPath.left( gitPath.length() - 1 );
	}

	if( fi->path.startsWith( gitPath ) )
	{
		fi->type = UnknownFile;

		QString gitLocalFile = fi->fullPath().mid( gitPath.length() + 1 );
		#if DBG_FSWATCHER
		qDebug( "Git-File: local=%s", qPrintable( gitLocalFile ) );
		#endif

		if( gitLocalFile.startsWith( QLatin1String( "refs" ) ) ||
			gitLocalFile == QLatin1String( "packed-refs" ) )
		{
			fi->type = RefsFile;
		}
		else if( gitLocalFile.startsWith( QLatin1String( "logs" ) ) )
		{
			fi->type = RefLogFile;
		}
		else if( gitLocalFile == QLatin1String( "index" ) )
		{
			fi->type = IndexFile;
		}
		else if( gitLocalFile == QLatin1String( "config" ) )
		{
			fi->type = ConfigFile;
		}
		else if( gitLocalFile == QLatin1String( "description" ) )
		{
			fi->type = DescriptionFile;
		}
		else if( gitLocalFile == QLatin1String( "HEAD" ) )
		{
			fi->type = HeadFile;
		}
	}
	else
	{
		if( fi->name == QLatin1String( ".gitattributes" ) ||
			fi->name == QLatin1String( ".gitignore" ) ||
			fi->name == QLatin1String( ".gitmodules" ) )
		{
			fi->type = RepoGitFile;
		}
		else
		{
			fi->type = WorkingTreeFile;
		}
	}

	path->children.insert( name, fi );

	#if DBG_FSWATCHER
	qDebug( "Found File in '%s', named '%s', type %i", qPrintable( path->fullPath() ),
			qPrintable( name ), int( fi->type ) );
	#endif
}

void FSWatcherPrivate::lostFile( FileInfo* path, const QString& name )
{
	Q_ASSERT( path );

	#if DBG_FSWATCHER
	qDebug( "Lost File in '%s', named '%s'", qPrintable( path->fullPath() ), qPrintable( name ) );
	#else
	qDebug( "Unsupported: Loosing files in FSWatcher" );
	#endif
}

void FSWatcherPrivate::foundDirectory( FileInfo* path, const QString& name )
{
	Q_ASSERT( path );

	#if DBG_FSWATCHER
	qDebug( "Found Dir in '%s', named '%s'", qPrintable( path->fullPath() ), qPrintable( name ) );
	#endif

	FileInfo* fi = new FileInfo();
	fi->type = DirectoryFile;
	fi->path = path->fullPath();
	fi->name = name;
	fi->parent = path;
	path->children.insert( name, fi );
	seekLostAndFoundFiles( fi );

	mWatcher->addPath( fi->fullPath() );
}

void FSWatcherPrivate::lostDirectory( FileInfo* path )
{
	Q_ASSERT( path );

	#if DBG_FSWATCHER
	qDebug( "Lost Dir '%s'", qPrintable( path->fullPath() ) );
	#endif

	enqueueFood( WorkingTreeFile );

	foreach( FileInfo* fi, path->children )
	{
		if( fi->type == DirectoryFile )
		{
			lostDirectory( fi );
		}
		else
		{
			lostFile( path, fi->name );
		}
	}

	if( path->parent )
	{
		path->parent->children.remove( path->name );
	}
	delete path;
}

void FSWatcherPrivate::modifiedFile( FileInfo* path )
{
	Q_ASSERT( path );

	#if DBG_FSWATCHER
	qDebug( "Found modification of %s type = %i",
			qPrintable( path->fullPath() ), int( path->type ) );
	#endif

	enqueueFood( path->type );
}

void FSWatcherPrivate::enqueueFood( FileTypes types )
{
	mCurrentChange |= types;

	if( !mSpitQueued )
	{
		QTimer::singleShot( 500, mOwner, SLOT(spitOutChanges()) );
		mSpitQueued = true;
	}
}

void FSWatcherPrivate::spoonFeed()
{
	mSpitQueued = false;

	FileTypes spitNow = mLastChange &~ mCurrentChange;

	if( spitNow == 0 )
	{
		mLastChange = mCurrentChange;
		mCurrentChange = 0;
		QTimer::singleShot( 500, mOwner, SLOT(spitOutChanges()) );
		mSpitQueued = true;
		return;
	}

	mLastChange = mCurrentChange &~ spitNow;
	mCurrentChange = 0;
	if( mLastChange != 0 )
	{
		QTimer::singleShot( 500, mOwner, SLOT(spitOutChanges()) );
		mSpitQueued = true;
	}

	#if DBG_FSWATCHER
	qDebug( "Spoon feeding the application with %i", int( spitNow ) );
	#endif

	if( spitNow & WorkingTreeFile )
	{
		emit mOwner->workingTreeChanged();
	}

	if( spitNow & ConfigFile )
	{
		emit mOwner->configChanged();
	}

	if( spitNow & RefsFile )
	{
		emit mOwner->refsChanged();
	}

	if( spitNow & HeadFile )
	{
		emit mOwner->headChanged();
	}

	if( spitNow & ModeFile )
	{
		emit mOwner->modeChanged();
	}

	if( spitNow & IndexFile )
	{
		emit mOwner->indexChanged();
	}

	if( spitNow & DescriptionFile )
	{
		emit mOwner->descriptionChanged();
	}

	if( spitNow & RefLogFile )
	{
		emit mOwner->refLogChanged();
	}

	if( spitNow & RepoGitFile )
	{
		emit mOwner->repoGitFileChanged();
	}
}


FileInfo::FileInfo()
{
}

FileInfo::~FileInfo()
{
	qDeleteAll( children );
}

FSWatcher::FSWatcher( QObject* parent )
	: QObject( parent )
	, d( NULL )
{
	d = new FSWatcherPrivate( this );
}

FSWatcher::~FSWatcher()
{
	delete d;
}

void FSWatcher::setRepository( Git::Repository repo )
{
	if( d->mRepo.isValid() )
	{
		delete d->mRoot;
		delete d->mWatcher;
		d->mWatcher = NULL;
		d->mRoot = NULL;
	}

	d->mRepo = repo;

	if( d->mRepo.isValid() )
	{
		d->mWatcher = new QFileSystemWatcher( this );

		connect( d->mWatcher, SIGNAL(directoryChanged(QString)),
				 this, SLOT(directoryChanged(QString)) );

		d->mRoot = d->mkFileInfoForPath( d->mRepo.basePath() );
		d->mWatcher->addPath( d->mRepo.basePath() );
		d->foundDirectory( d->mRoot, QLatin1String( ".git" ) );
		d->seekLostAndFoundFiles( d->mRoot );
	}
}

void FSWatcher::directoryChanged( const QString& path )
{
	#if DBG_FSWATCHER
	qDebug( "Notify for: '%s'", qPrintable( path ) );
	#endif

	QString repoPath = path.mid( d->mRepo.basePath().length() );
	FileInfo* fi = d->mRoot;
	if( !repoPath.isEmpty() )
	{
		QStringList sl = repoPath.split( L'/' );
		foreach( QString sub, sl )
		{
			FileInfo* fi2 = fi->children.value( sub, NULL );
			if( !fi2 )
			{
				qDebug( "Is not known." );
				return;
			}
			fi = fi2;
		}
	}
	#if DBG_FSWATCHER
	qDebug( "Found as %s", qPrintable( fi->fullPath() ) );
	#endif

	if( QDir( path ).exists() )
	{
		d->seekLostAndFoundFiles( fi );
	}
	else
	{
		d->lostDirectory( fi );
	}
}

void FSWatcher::spitOutChanges()
{
	d->spoonFeed();
}
