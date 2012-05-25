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
#include "ObjectId.h"
#include "Reference.h"
#include "RefSpec.h"
#include "Git_p.h"

namespace Git
{

	RemotePrivate::RemotePrivate( RepositoryPrivate* repo, git_remote* remote )
		: mRepo( repo )
		, mRemote( remote )
	{
		Q_ASSERT( mRepo );
		mRepo->ref();
	}

	RemotePrivate::~RemotePrivate()
	{
		git_remote_free( mRemote );

		if( mRepo )
		{
			mRepo->deref();
		}
	}

	Remote::Remote()
	{
	}

	Remote::Remote( RemotePrivate* _d )
		: d( _d )
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

	void Remote::save()
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_save( d->mRemote );
		}
	}

	QByteArray Remote::name() const
	{
		QByteArray a;
		if( d )
		{
			a = QByteArray( git_remote_name( d->mRemote ) );
		}
		return a;
	}

	QByteArray Remote::url() const
	{
		QByteArray a;
		if( d )
		{
			a = QByteArray( git_remote_url( d->mRemote ) );
		}
		return a;
	}

	void Remote::setFetchSpec( const QByteArray& spec )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_set_fetchspec( d->mRemote, spec.constData() );
		}
	}

	void Remote::setPushSpec( const QByteArray& spec )
	{
		Q_ASSERT( d );
		if( d )
		{
			int rc = git_remote_set_pushspec( d->mRemote, spec.constData() );
		}
	}

	RefSpec Remote::fetchSpec() const
	{
		if( d )
		{
			return mkRefSpec( git_remote_fetchspec( d->mRemote ) );
		}

		return RefSpec();
	}

	RefSpec Remote::pushSpec() const
	{
		if( d )
		{
			return mkRefSpec( git_remote_pushspec( d->mRemote ) );
		}

		return RefSpec();
	}

	bool Remote::isValidUrl( const QByteArray& url )
	{
		return git_remote_valid_url( url.constData() );
	}

	bool Remote::isSupportedUrl( const QByteArray& url )
	{
		return git_remote_supported_url( url.constData() );
	}

}

