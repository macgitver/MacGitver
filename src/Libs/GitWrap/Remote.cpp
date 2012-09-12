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

#include "Remote.h"
#include "RemotePrivate.h"
#include "Reference.h"
#include "RefSpec.h"

namespace Git
{

	BEGIN_INTERNAL_IMPL()

	RemotePrivate::RemotePrivate( RepositoryPrivate* repo, git_remote* remote )
		: RepoObject( repo )
		, mRemote( remote )
	{
		Q_ASSERT( remote );
	}

	RemotePrivate::~RemotePrivate()
	{
		git_remote_free( mRemote );
	}

	END_INTERNAL_IMPL()

	Remote::Remote()
	{
	}

	Remote::Remote( Internal::RemotePrivate* _d )
		: d( _d )
	{
	}

	Remote::Remote( const Remote& other )
		: d( other.d )
	{
	}

	Remote::~Remote()
	{
	}

	Remote& Remote::operator=( const Remote& other )
	{
		d = other.d;
		return * this;
	}

	bool Remote::isValid() const
	{
		return d;
	}

	bool Remote::save()
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_save( d->mRemote );
			return d->handleErrors( rc );
		}
		return false;
	}

	QString Remote::name() const
	{
		if( d )
		{
			return QString::fromUtf8( git_remote_name( d->mRemote ) );
		}
		return QString();
	}

	QString Remote::url() const
	{
		if( d )
		{
			return QString::fromUtf8( git_remote_url( d->mRemote ) );
		}
		return QString();
	}

	bool Remote::setFetchSpec( const QString& spec )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_set_fetchspec( d->mRemote, spec.toUtf8().constData() );
			return d->handleErrors( rc );
		}

		return false;
	}

	bool Remote::setPushSpec( const QString& spec )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_set_pushspec( d->mRemote, spec.toUtf8().constData() );
			return d->handleErrors( rc );
		}

		return false;
	}

	RefSpec Remote::fetchSpec() const
	{
		if( d )
		{
			return Internal::mkRefSpec( git_remote_fetchspec( d->mRemote ) );
		}

		return RefSpec();
	}

	RefSpec Remote::pushSpec() const
	{
		if( d )
		{
			return Internal::mkRefSpec( git_remote_pushspec( d->mRemote ) );
		}

		return RefSpec();
	}

	bool Remote::isValidUrl( const QString& url )
	{
		return git_remote_valid_url( url.toUtf8().constData() );
	}

	bool Remote::isSupportedUrl( const QString& url )
	{
		return git_remote_supported_url( url.toUtf8().constData() );
	}


	bool Remote::connect( bool forFetch )
	{
		if( d )
		{
			int rc = git_remote_connect( d->mRemote, forFetch ? GIT_DIR_FETCH : GIT_DIR_PUSH );
			return d->handleErrors( rc );
		}

		return false;
	}

	void Remote::disconnect()
	{
		if( d )
		{
			git_remote_disconnect( d->mRemote );
		}
	}

	bool Remote::download()
	{
		if( d )
		{
			int rc = git_remote_download( d->mRemote, &d->mBytes, &d->mStats );
			return d->handleErrors( rc );
		}

		return false;
	}

	bool Remote::updateTips()
	{
		if( d )
		{
			int rc = git_remote_update_tips( d->mRemote );
			return d->handleErrors( rc );
		}

		return false;
	}

}

