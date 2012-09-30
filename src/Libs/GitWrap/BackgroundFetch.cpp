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

#include "BackgroundFetch.h"
#include "Result.h"

namespace Git
{

	BackgroundFetch::BackgroundFetch()
	{
	}

	void BackgroundFetch::setRepository( Repository repo )
	{
		mRepo = repo;
	}

	void BackgroundFetch::setRemote( Remote remote )
	{
		mRemote = remote;
	}

	bool BackgroundFetch::execute()
	{
		Result r;

		if( !mRemote.connect( true, r ) )
		{
			return false;
		}

		if( !mRemote.download( r ) )
		{
			return false;
		}

		if( !mRemote.updateTips( r ) )
		{
			return false;
		}

		mRemote.disconnect( r );
		return true;
	}

}

