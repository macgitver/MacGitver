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

#include "Config.h"
#include "ConfigPrivate.h"

namespace Git
{

	ConfigPrivate::ConfigPrivate( git_config* cfg )
		: mCfg( cfg )
	{
	}

	ConfigPrivate::~ConfigPrivate()
	{
		git_config_free( mCfg );
	}

	void ConfigPrivate::ref()
	{
		mRefCounter.ref();
	}

	void ConfigPrivate::deref()
	{
		if( !mRefCounter.deref() )
		{
			delete this;
		}
	}


	Config::Config()
	{
	}

	Config::Config( ConfigPrivate* cfg )
		: d( cfg )
	{
	}

	Config::Config( const Config& other )
		: d( other.d )
	{
	}

	Config::~Config()
	{
	}

	Config& Config::operator=( const Config& other )
	{
		d = other.d;
		return * this;
	}

	QString Config::globalFilePath()
	{
		QString filePath;
		char path[ 2048 ];

		int rc = git_config_find_system( path, 2048 - 1 );
		if( rc >= 0 )
		{
			filePath = QString::fromLocal8Bit( path );
		}

		return filePath;
	}

	QString Config::userFilePath()
	{
		QString filePath;
		char path[ 2048 ];

		int rc = git_config_find_global( path, 2048 - 1 );
		if( rc >= 0 )
		{
			filePath = QString::fromLocal8Bit( path );
		}

		return filePath;
	}

	Config Config::global()
	{
		char path[ 2048 ];

		int rc = git_config_find_system( path, 2048 - 1 );
		if( rc < 0 )
		{
			return Config();
		}

		git_config* cfg = NULL;
		rc = git_config_open_ondisk( &cfg, path );
		if( rc < 0 )
		{
			return Config();
		}

		return new ConfigPrivate( cfg );
	}

	Config Config::user()
	{
		char path[ 2048 ];

		int rc = git_config_find_global( path, 2048 - 1 );
		if( rc < 0 )
		{
			return Config();
		}

		git_config* cfg = NULL;
		rc = git_config_open_ondisk( &cfg, path );
		if( rc < 0 )
		{
			return Config();
		}

		return new ConfigPrivate( cfg );
	}

	Config Config::file( const QString& fileName )
	{
		git_config* cfg = NULL;

		int rc = git_config_open_ondisk( &cfg, fileName.toLocal8Bit().constData() );

		if( rc < 0 )
		{
			return Config();
		}

		return new ConfigPrivate( cfg );
	}

	Config Config::create()
	{
		git_config* cfg = NULL;
		int rc = git_config_new( &cfg );

		if( rc < 0 )
		{
			return Config();
		}

		return new ConfigPrivate( cfg );
	}

	bool Config::addFile( const QString& fileName, int priority )
	{
		if( !d || fileName.isEmpty() )
		{
			return false;
		}

		int rc = git_config_add_file_ondisk( d->mCfg, fileName.toLocal8Bit().constData(), priority );
		if( rc < 0 )
		{
			return false;
		}

		return true;
	}

	static int read_config_cb( const char* key, const char* value, void* data )
	{
		ConfigValues* cv = (ConfigValues*) data;
		cv->insert( QString::fromUtf8( key ), QString::fromUtf8( value ) );
		return 0;
	}

	ConfigValues Config::values()
	{
		ConfigValues values;
		git_config_foreach( d->mCfg, &read_config_cb, (void*) &values );
		return values;
	}

}

