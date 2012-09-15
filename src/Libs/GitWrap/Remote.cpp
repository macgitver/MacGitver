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

	namespace Internal
	{

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

	}

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

	bool Remote::save( Result& result )
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

		result = git_remote_save( d->mRemote );
		return result;
	}

	QString Remote::name() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return QString();
		}

		return QString::fromUtf8( git_remote_name( d->mRemote ) );
	}

	QString Remote::url() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return QString();
		}

		return QString::fromUtf8( git_remote_url( d->mRemote ) );
	}

	bool Remote::setFetchSpec( const QString& spec, Result& result )
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

		result = git_remote_set_fetchspec( d->mRemote, spec.toUtf8().constData() );
		return result;
	}

	bool Remote::setPushSpec( const QString& spec, Result& result )
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

		result = git_remote_set_pushspec( d->mRemote, spec.toUtf8().constData() );
		return result;
	}

	RefSpec Remote::fetchSpec() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return RefSpec();
		}

		return Internal::mkRefSpec( git_remote_fetchspec( d->mRemote ) );
	}

	RefSpec Remote::pushSpec() const
	{
		if( !d )
		{
			GitWrap::lastResult().setInvalidObject();
			return RefSpec();
		}

		return Internal::mkRefSpec( git_remote_pushspec( d->mRemote ) );
	}

	bool Remote::isValidUrl( const QString& url )
	{
		return git_remote_valid_url( url.toUtf8().constData() );
	}

	bool Remote::isSupportedUrl( const QString& url )
	{
		return git_remote_supported_url( url.toUtf8().constData() );
	}


	bool Remote::connect( bool forFetch, Result& result )
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

		result = git_remote_connect( d->mRemote, forFetch ? GIT_DIR_FETCH : GIT_DIR_PUSH );
		return result;
	}

	void Remote::disconnect( Result& result )
	{
		if( !result )
		{
			return;
		}

		if( !d )
		{
			result.setInvalidObject();
			return;
		}

		git_remote_disconnect( d->mRemote );
	}

	bool Remote::download( Result& result )
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

		result = git_remote_download( d->mRemote, &d->mBytes, &d->mStats );
		return result;
	}

	bool Remote::updateTips( Result& result )
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

		result = git_remote_update_tips( d->mRemote );
		return result;
	}

}

