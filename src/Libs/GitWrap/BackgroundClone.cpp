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

#include "BackgroundClone.h"
#include "BackgroundFetch.h"
#include "Remote.h"

namespace Git
{

	BackgroundClone::BackgroundClone()
	{
		mPrepared = false;
	}

	void BackgroundClone::setFrom( const QString& url, const QString& path )
	{
		mUrl = url;
		mPath = path;
	}

	void BackgroundClone::setRemoteName( const QString& remoteName )
	{
		mRemoteName = remoteName;
	}

	void BackgroundClone::setType( bool bare, bool mirror, bool checkout )
	{
		mBare = bare;
		mMirror = mirror;
		mCheckout = checkout;
	}

	void BackgroundClone::setBranch( const QString& name, bool onlyThisOne )
	{
		mCheckoutBranch = name;
		mFetchOnlyBranch = onlyThisOne;
	}

	void BackgroundClone::setSubmodule( bool init, bool recursive )
	{
		mInitSubmodules = init;
		mInitSubmodulesRecursive = recursive;
	}

	Repository BackgroundClone::repository()
	{
		if( !mPrepared )
		{
			prepare();
		}
		return mRepo;
	}

	bool BackgroundClone::prepare()
	{
		if( !mPrepared )
		{
			mRepo = Repository::create( mPath, mBare );
			mPrepared = true;
		}

		return mRepo.isValid();
	}

	bool BackgroundClone::execute()
	{
		if( !mPrepared )
		{
			if( !prepare() )
			{
				return false;
			}
		}
		if( !mRepo.isValid() )
		{
			return false;
		}

		QString refSpec = QLatin1String( "+refs/heads/*:refs/remotes/%1/*" );

		Remote remote = mRepo.createRemote( mRemoteName, mUrl, refSpec.arg( mRemoteName ) );

		if( !remote.isValid() )
		{
			return false;
		}

		remote.save();

		BackgroundFetch* fetch = new BackgroundFetch();
		fetch->setRepository( mRepo );
		fetch->setRemote( remote );

		worker()->queue( fetch );

		return true;
	}


}
